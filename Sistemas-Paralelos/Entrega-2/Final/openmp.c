#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de matrices
int m=4;

//Cantidad de threads por defecto
int T=4;

double escalar;
double max, min, suma;
double *R;
double** M;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 int i,j,c,k,tid;
 double timetick;
 double sumaLocal,minLocal,maxLocal;
 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((m = atoi(argv[2])) <= 0))
  {
    printf("\nUsar: %s n m\n  n: Dimension de la matriz (nxn X nxn)\n  m: Cantidad de matrices\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  R=(double*)malloc(sizeof(double)*N*N);

  M=(double**)malloc(sizeof(double*)*m);
  for(i=0;i<m;i++){
    M[i]=(double*)malloc(sizeof(double)*N*N);
  }

 //Inicializa las matrices
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
  
  minLocal=9999;
  maxLocal=-999;
  sumaLocal=0;

 //Realiza el calculo
  timetick = dwalltime();
  
  omp_set_num_threads(T);  

  #pragma omp parallel private(tid,i,j,c,k) shared(M,R,min,max) firstprivate(sumaLocal,minLocal,maxLocal) reduction(+: suma)
  {
    tid = omp_get_thread_num();
    #pragma omp for
    for(i=0;i<N;++i){
      for(j=0;j<N;++j){
        R[i*N+j]=0;
        if(M[0][i*N+j] < minLocal) minLocal=M[0][i*N+j];
        if(M[0][i*N+j] > maxLocal) maxLocal=M[0][i*N+j];
        if(M[1][i*N+j] < minLocal) minLocal=M[1][i*N+j];
        if(M[1][i*N+j] > maxLocal) maxLocal=M[1][i*N+j];
        sumaLocal+=M[0][i*N+j]+M[1][i*N+j];
        for(c=0;c<N;++c){
          R[i*N+j]+=M[0][i*N+c]*M[1][c+j*N];
        }
      }
    }

    for(k=2;k<m;++k){
      if(k%2==1){
        #pragma omp for
        for(i=0;i<N;++i){
          for(j=0;j<N;++j){
            R[i*N+j]=0;
            if(M[k][i*N+j] < minLocal) minLocal=M[k][i*N+j];
            if(M[k][i*N+j] > maxLocal) maxLocal=M[k][i*N+j];
            sumaLocal+=M[k][i*N+j];
            for(c=0;c<N;++c){
              R[i*N+j]+=M[0][i*N+c]*M[k][c+j*N];
            }
          }
        }
      } else {
        #pragma omp for
        for(i=0;i<N;++i){
          for(j=0;j<N;++j){
            M[0][i*N+j]=0;
            if(M[k][i*N+j] < minLocal) minLocal=M[k][i*N+j];
            if(M[k][i*N+j] > maxLocal) maxLocal=M[k][i*N+j];
            sumaLocal+=M[k][i*N+j];
            for(c=0;c<N;++c){
              M[0][i*N+j]+=R[i*N+c]*M[k][c+j*N];
            }
          }
        }
      }
    }
    
    #pragma omp critical(min)
    {
      if(minLocal<min) min = minLocal;
    }

    #pragma omp critical(max)
    {
      if(maxLocal>max) max = maxLocal;
    }

    suma = sumaLocal;
  }

  if(m%2 == 1){
    free(R);
    R=M[0];
  }
  escalar=pow(((max-min)/(suma/(N*N*m))),m);

  #pragma omp parallel private(i,j) shared(R)
  {
    #pragma omp for
    for(i=0;i<N;++i){
      for(j=0;j<N;++j){
        R[i*N+j] *= escalar;
      }
    }
  }

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  free(R);
  for(i=1;i<m;i++){
    free(M[i]);
  }
  free(M);
 return(0);
}
