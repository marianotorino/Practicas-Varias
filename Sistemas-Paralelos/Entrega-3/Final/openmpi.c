#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

int N=100;
int T=4;
int m=4;

void master(int id){
  //Para calcular tiempo
  double dwalltime(){
          double sec;
          struct timeval tv;

          gettimeofday(&tv,NULL);
          sec = tv.tv_sec + tv.tv_usec/1000000.0;
          return sec;
  }

  double** M;
  double* R;
  double min,max,suma;

  int i,j,k;

  M=(double**)malloc(sizeof(double*)*m);
  for(i=0;i<m;i++){
    M[i]=(double*)malloc(sizeof(double)*N*N);
  }

 //Inicializa matrices
  for(k=0;k<m;++k){
   for(i=0;i<N;++i){
     for(j=0;j<N;++j){
       M[k][i*N+j] = (double) (rand() % 1000000000);
     }
   }
 }

  min=9999;
  max=-999;
  suma=0;

  MPI_Status estado;

  R=(double*) malloc(sizeof(double)*N*N);

  //Realiza el calculo
  double timetick = dwalltime();

  int cant = (T-1)*(m+1);
  double datos[3];
  int cantFilas = N/(T-1);
  int tid;
  int* cod=(int*)malloc(sizeof(int));

  for(i=0;i<cant;i++){
    MPI_Recv(cod, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado);
    tid = estado.MPI_SOURCE;
    if(*cod == 0){
      MPI_Send(&(M[0][(tid-1)*N*cantFilas]), N*cantFilas, MPI_DOUBLE, tid, 1, MPI_COMM_WORLD);
    }else if(*cod != -1){
      MPI_Send(M[*cod], N*N, MPI_DOUBLE, tid, 2, MPI_COMM_WORLD);
    }else{
      MPI_Recv(&datos, 3, MPI_DOUBLE, tid, 3, MPI_COMM_WORLD, &estado);
      if(datos[0]<min) min=datos[0];
      if(datos[1]>max) max=datos[1];
      suma+=datos[2];
    }
  }

  // Calculo el escalar
  //double escalar=pow(((max-min)/(suma/(N*N*m))),m);

  double* escalar=(double*)malloc(sizeof(double));
  *escalar=pow(((max-min)/(suma/(N*N*m))),m);
  MPI_Bcast(escalar, 1, MPI_DOUBLE, id, MPI_COMM_WORLD);

  for(i=1;i<T;i++){
    // Recibo resultados
    MPI_Probe(MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &estado);
    tid = estado.MPI_SOURCE;
    //printf("Process %d\n",tid);

    MPI_Recv(&(R[(tid-1)*N*cantFilas]), N*cantFilas, MPI_DOUBLE, tid, 5, MPI_COMM_WORLD, &estado);
  }
  
  printf("Tiempo en segundos %f\n", dwalltime() - timetick);

/*
  printf("Resultado:\n");
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      printf("%f ",R[i*N+j]);
    }
    printf("\n");
  }
*/
  free(escalar);
  for(i=0;i<m;i++){
    free(M[i]);
  }
  free(M);
  free(R);
}

void worker(int id){
  double sumaLocal,minLocal,maxLocal;
  int i,j,k,c;
  double** M;
  double* R;
  double* L;
  minLocal=9999;
  maxLocal=-999;
  sumaLocal=0;
  int dim = N/(T-1);
  int fin = ((id-1)*(dim))+dim-1;
  int cantM = m-1;

  MPI_Status estado;

  M=(double**)malloc(sizeof(double*)*cantM);
  for(i=0;i<cantM;i++){
    M[i]=(double*)malloc(sizeof(double)*N*N);
  }

  L=(double*)malloc(sizeof(double)*N*dim);

  R=(double*)malloc(sizeof(double)*N*dim);

  int cod = 0;

  // Pedir su fragmento de matriz M[0] y lo guardo en L
  MPI_Send(&cod, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  MPI_Recv(&L[0], N*dim, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &estado);
 
  // Pedir matriz M[1]
  cod++;
  MPI_Send(&cod, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  MPI_Recv(M[0], N*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &estado);

  for(i=0;i<dim;i++){
    for(j=0;j<N;j++){
      R[i*N+j]=0;
      if(L[i*N+j] < minLocal) minLocal=L[i*N+j];
      if(L[i*N+j] > maxLocal) maxLocal=L[i*N+j];
      if(M[0][(fin-i)*N+j] < minLocal) minLocal=M[0][(fin-i)*N+j];
      if(M[0][(fin-i)*N+j] > maxLocal) maxLocal=M[0][(fin-i)*N+j];
      sumaLocal+=L[i*N+j]+M[0][(fin-i)*N+j];
      for(c=0;c<N;++c){
        R[i*N+j]+=L[i*N+c]*M[0][c+j*N];
      }
    }
  }

  for(k=1;k<cantM;k++) {
    // Pedir matriz M[k+1]
    cod = k+1;
    MPI_Send(&cod, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(M[k], N*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &estado);  

    if(k%2==0){
      for(i=0;i<dim;i++){
        for(j=0;j<N;j++){
          R[i*N+j]=0;
          if(M[k][(fin-i)*N+j] < minLocal) minLocal=M[k][(fin-i)*N+j];
          if(M[k][(fin-i)*N+j] > maxLocal) maxLocal=M[k][(fin-i)*N+j];
          sumaLocal+=M[k][(fin-i)*N+j];
          for(c=0;c<N;++c){
            R[i*N+j]+=M[0][i*N+c]*M[k][c+j*N];
          }
        }
      }
    } else {
      for(i=0;i<dim;++i){
        for(j=0;j<N;++j){
          L[i*N+j]=0;
          if(M[k][(fin-i)*N+j] < minLocal) minLocal=M[k][(fin-i)*N+j];
          if(M[k][(fin-i)*N+j] > maxLocal) maxLocal=M[k][(fin-i)*N+j];
          sumaLocal+=M[k][(fin-i)*N+j];
          for(c=0;c<N;++c){
            L[i*N+j]+=R[i*N+c]*M[k][c+j*N];
          }
        }
      }
    }
  }

  if(cantM%2 == 0){
    R=L;
  }

  double datos[3];
  datos[0]=minLocal;
  datos[1]=maxLocal;
  datos[2]=sumaLocal;

  // Envio mi maxLocal, minLocal y sumaLocal
  cod = -1;
  MPI_Send(&cod, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  MPI_Send(&datos, 3, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);

  double* escalar = (double*)malloc(sizeof(double));

  MPI_Bcast(escalar, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

 // MPI_Recv(escalar, 1, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD, &estado);

  for(i=0;i<dim;++i){
    for(j=0;j<N;++j){
      R[i*N+j] *= *escalar;
    }
  }

  // Envio resultado
  MPI_Send(R, N*dim, MPI_DOUBLE, 0, 5, MPI_COMM_WORLD);

  free(escalar);

  for(i=0;i<cantM;i++){
    free(M[i]);
  }
  free(M);

  free(R);
}

void main(int argc,char*argv[]) {
  int id;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD,&T);

  if ((argc != 4) || ((N = atoi(argv[1])) <= 0) || ((m = atoi(argv[2])) <= 0) || ((T = atoi(argv[3])) <= 0))
  {
    printf("\nUsar: %s n m t\n  n: Dimension de la matriz (nxn X nxn)\n  m: Cantidad de matrices\n  t: Cantidad de procesos\n", argv[0]);
    exit(1);
  }

  if(id == 0) {
    master(id);
  } else {
    worker(id);
  }

  MPI_Finalize();

}
