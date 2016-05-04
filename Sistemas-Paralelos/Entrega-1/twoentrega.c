#include<stdio.h>
#include<stdlib.h>

//Dimension por defecto de las matrices
int N=100;

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

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);
  D=(double*)malloc(sizeof(double)*N*N);
  AB=(double*)malloc(sizeof(double)*N*N);
  CA=(double*)malloc(sizeof(double)*N*N);
  BD=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	  A[i*N+j]=1;
	  B[i+j*N]=1;
    C[i*N+j]=1;
	  D[i+j*N]=1;
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
    AB[i*N+j]=0;
    for(k=0;k<N;k++){
	    AB[i*N+j] += A[i*N+k] * B[k+j*N];
    }
   }
  }   

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    CA[i*N+j]=0;
    for(k=0;k<N;k++){
	    CA[i*N+j] += C[i*N+k] * A[j*N+k];
    }
  }
}

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    BD[i*N+j]=0;
    for(k=0;k<N;k++){
	    BD[i*N+j] += B[k+i*N] * D[k+j*N];
    }
  }
}

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      AB[i*N+j] += CA[i*N+j] + BD[i*N+j];
    }
  }

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
