#include<stdio.h>
#include<stdlib.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de threads por defecto
int T=2;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 double *A,*B,*R;
 int i,j,k;
 int check=1;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0))
  {
    printf("\nUsar: %s n t\n  n: Dimension de la matriz (nxn X nxn)\n  t: Cantidad de threads\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  R=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	  A[i*N+j]=1;
	  B[i+j*N]=1;
   }
  }   

  void printMatrix(double *m, char *nombre, int t){
    int i,j;
    printf("Matriz %s\n", nombre);
    if(t == 0){
      for(i=0;i<N;i++){
        for(j=0;j<N;j++){
          printf("%f ", m[i*N+j]);      
        }
        printf("\n");
      }
    } else {
    for(i=0;i<N;i++){
      for(j=0;j<N;j++){
        printf("%f ", m[i+j*N]);      
      }
      printf("\n");
    }
  }
 }

 //Realiza la multiplicacion

  timetick = dwalltime();

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      R[i*N+j]=0;
      for(k=0;k<N;k++){
	R[i*N+j]+=A[i*N+k] * B[k+j*N];
      }
    }
  }

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(R[i*N+j] == N);
   }
  }   

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }

 free(A);
 free(B);
 free(R);
 return(0);
}
