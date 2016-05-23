#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de matrices
int m=3;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 double *R;
 int i,j,k,c;
 int check=1;
 double max,min,avg,escalar,temp;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  R=(double*)malloc(sizeof(double)*N*N);

  double* M[m];
  for(i=0;i<m;i++){
    M[i]=(double*)malloc(sizeof(double)*N*N);
  }

 //printMatrix
void printMatrix(double* M, char* c, int ord){
  printf("Matriz %s:\n", c);
  if(ord==1){
    for (i=0;i<N;i++){
      for(j=0;j<N;j++){
        printf("%f ",M[i+j*N]);
      }
      printf("\n");
    }
  } else {
    for (i=0;i<N;i++){
      for(j=0;j<N;j++){
        printf("%f ",M[i*N+j]);
      }
      printf("\n");
    }
  }
}
 //Inicializa las matrices
 //matriz A
  M[0][0]=4;
  M[0][1]=5;
  M[0][2]=6;
  M[0][3]=7;
  M[0][4]=8;
  M[0][5]=9;
  M[0][6]=10;
  M[0][7]=11;
  M[0][8]=12;
 //matriz B
  M[1][0]=5;
  M[1][1]=2;
  M[1][2]=8;
  M[1][3]=4;
  M[1][4]=1;
  M[1][5]=7;
  M[1][6]=3;
  M[1][7]=0;
  M[1][8]=6;
 //matriz C
  M[2][0]=8;
  M[2][1]=7;
  M[2][2]=6;
  M[2][3]=5;
  M[2][4]=4;
  M[2][5]=3;
  M[2][6]=2;
  M[2][7]=1;
  M[2][8]=0;

printMatrix(M[0],"A",0);
printMatrix(M[1],"B",1);
printMatrix(M[2],"C",1);
 //Realiza la multiplicacion

  timetick = dwalltime();

  double cantElem=N*N*m;

//calculo de matrices
  min=999999999;
  max=-1;
  avg=0;

//productoria
  
  for(i=0;i<N;++i){
    for(j=0;j<N;++j){
      R[i*N+j]=0;
      if(M[0][i*N+j] < min) min=M[0][i*N+j];
      if(M[0][i*N+j] > max) max=M[0][i*N+j];
      if(M[1][i*N+j] < min) min=M[1][i*N+j];
      if(M[1][i*N+j] > max) max=M[1][i*N+j];
      avg+=M[0][i*N+j]+M[1][i*N+j];
      for(c=0;c<N;++c){
        R[i*N+j]+=M[0][i*N+c]*M[1][c+j*N];
      }
    }
  }
  
  for(k=2;k<m;++k){
    if(k%2==1){
      for(i=0;i<N;++i){
        for(j=0;j<N;++j){
          R[i*N+j]=0;
          if(M[k][i*N+j] < min) min=M[k][i*N+j];
          if(M[k][i*N+j] > max) max=M[k][i*N+j];
          avg+=M[k][i*N+j];
          for(c=0;c<N;++c){
            R[i*N+j]+=M[0][i*N+c]*M[k][c+j*N];
          }
        }
      }
    } else {
      for(i=0;i<N;++i){
        for(j=0;j<N;++j){
          M[0][i*N+j]=0;
          if(M[k][i*N+j] < min) min=M[k][i*N+j];
          if(M[k][i*N+j] > max) max=M[k][i*N+j];
          avg+=M[k][i*N+j];
          for(c=0;c<N;++c){
            M[0][i*N+j]+=R[i*N+c]*M[k][c+j*N];
          }
        }
      }
    }
  }

//if m es par el resultado esta en M[0] sino en R   
  if(m%2 == 1){
    free(R);
    R=M[0];
  }
//multiplicacion del resultado por el escalar elevado a la m
  avg/=cantElem;
  escalar=pow(((max-min)/avg),m);
  for(i=0;i<N;++i){
    for(j=0;j<N;++j){
      R[i*N+j] = R[i*N+j] * escalar;
    }
  }

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);
printMatrix(R,"R",0);
  free(R);
  for(i=1;i<m;++i){
    free(M[i]);
  }
 return(0);
}

