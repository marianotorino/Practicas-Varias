#include<stdio.h>
#include<stdlib.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de matrices
int m=4;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 double *M;
 int i,j,k,c;
 int check=1;
 double max,min,avg,escalar,temp;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((m = atoi(argv[2])) <= 0) )
  {
    printf("\nUsar: %s n m\n  n: Dimension de la matriz (nxn X nxn)\nm: Cantidad de matrices a procesar\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  M=(double*)malloc(sizeof(double)*N*N*M);
  R=(double*)malloc(sizeof(double)*N*N);
 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
 


 //Realiza la multiplicacion

  timetick = dwalltime();

  int cantElem=N*N;

  for(k=0;i<M;i++){
    min=9999;
    max=-1;
    avg=0;
    for(i=0;i<N;i++){
      for(j=0;j<N;j++){
        if(M[m*k+i*N+j] < min) min=M[m*k+i*N+j];
        if(M[m*k+i*N+j] > max) max=M[m*k+i*N+j];
        avg+=M[m*k+i*N+j];
      }
    }
    avg/=cantElem;
    escalar=(max-min)/avg;
    for(i=0;i<N;i++){
      for(j=0;j<N;j++){
        M[m*k+i*N+j]*=escalar;
      }
    }
  }

  for(k=0;k<M;k++){
    if(k%2==0){
      for(i=0;i<N;i++){
        for(j=0;j<N;j++){
          R[i*N+j]=0;
          for(c=0;c<N;c++){
            R[i*N+j]+=M[i*N+c]*M[m*k+c*N+j];
          }
        }
      }
    } else {
      for(i=0;i<N;i++){
        for(j=0;j<N;j++){
          M[i*N+j]=0;
          for(c=0;c<N;c++){
            M[i*N+j]+=R[i*N+c]*M[m*k+c*N+j];
          }
        }
      }
    }
  }

//if N es par el resultado esta en M[0] sino en R   


 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(AB[i*N+j] == 3*N);
   }
  }   

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }

 free(A);
 free(B);
 free(C);
 free(D);
 free(AB);
 free(CA);
 free(BD);
 return(0);
}
