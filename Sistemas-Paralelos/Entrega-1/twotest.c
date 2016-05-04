#include<stdio.h>
#include<stdlib.h>

//Dimension por defecto de las matrices
int N=4;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 double *A,*B,*C,*D,*AB,*CA,*BD;
 int i,j,k;
 int check=1;
 double timetick;

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);
  D=(double*)malloc(sizeof(double)*N*N);
  AB=(double*)malloc(sizeof(double)*N*N);
  CA=(double*)malloc(sizeof(double)*N*N);
  BD=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  int num = 1;
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	  A[i*N+j]=num;
	  B[i+j*N]=num+16;
    C[i*N+j]=num+32;
	  D[i+j*N]=num+48;
    num++;
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

  printMatrix(A, "A",0);
  printMatrix(B, "B",1);
  printMatrix(C, "C",0);
  printMatrix(D, "D",1);

 //Realiza la multiplicacion

  timetick = dwalltime();

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    AB[i*N+j]=0;
    for(k=0;k<N;k++){
	    AB[i*N+j] += A[i*N+k] * B[k+j*N];
    }
   }
  }   

 printMatrix(AB, "AB",0);

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    CA[i*N+j]=0;
    for(k=0;k<N;k++){
	    CA[i*N+j] += C[i*N+k] * A[j*N+k];
    }
  }
}

 printMatrix(CA, "CA",0);

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    BD[i*N+j]=0;
    for(k=0;k<N;k++){
	    BD[i*N+j] += B[k+i*N] * D[k+j*N];
    }
  }
}

 printMatrix(BD, "BD",0);

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      AB[i*N+j] += CA[i*N+j] + BD[i*N+j];
    }
  }

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  printMatrix(AB,"Resultado",0);

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
