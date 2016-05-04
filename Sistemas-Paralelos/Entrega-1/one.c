#include<stdio.h>
#include<stdlib.h>
#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1

//Dimension por defecto de las matrices
int N=100;

//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna,int orden){
 if(orden==ORDENXFILAS){
  return(matriz[fila*N+columna]);
 }else{
  return(matriz[fila+columna*N]);
 }
}

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz,int fila,int columna,int orden,double valor){
 if(orden==ORDENXFILAS){ 
  matriz[fila*N+columna]=valor;
 }else{
  matriz[fila+columna*N]=valor;
 }
}

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 double *A,*B,*C,*D,*AB,*CA,*BD,*R;
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
  R=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	setValor(A,i,j,ORDENXFILAS,1);
	setValor(B,i,j,ORDENXFILAS,1);
  setValor(C,i,j,ORDENXFILAS,1);
	setValor(D,i,j,ORDENXFILAS,1);
   }
  }   


 //Realiza la multiplicacion

  timetick = dwalltime();

  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    setValor(AB,i,j,ORDENXFILAS,0);
    setValor(CA,i,j,ORDENXFILAS,0);
    setValor(BD,i,j,ORDENXFILAS,0);
    for(k=0;k<N;k++){
	setValor(AB,i,j,ORDENXFILAS, getValor(AB,i,j,ORDENXFILAS) + getValor(A,i,k,ORDENXFILAS)*getValor(B,k,j,ORDENXFILAS));
  setValor(CA,i,j,ORDENXFILAS, getValor(CA,i,j,ORDENXFILAS) + getValor(C,i,k,ORDENXFILAS)*getValor(A,j,k,ORDENXFILAS));
  setValor(BD,i,j,ORDENXFILAS, getValor(BD,i,j,ORDENXFILAS) + getValor(B,k,i,ORDENXFILAS)*getValor(D,k,j,ORDENXFILAS));
    }
   }
  }   

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      setValor(R,i,j,ORDENXFILAS, getValor(AB,i,j,ORDENXFILAS) + getValor(CA,i,j,ORDENXFILAS) + getValor(BD,i,j,ORDENXFILAS));
    }
  }

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(getValor(R,i,j,ORDENXFILAS)== 3*N);
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
 free(R);
 return(0);
}
