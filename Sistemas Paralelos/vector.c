#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Dimension por defecto del vector
int N=256;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int printVector(double *v){
	int k;
	for(k=0;k<N;k=k+4){
		printf("%f %f %f %f\n",v[k], v[k+1], v[k+2], v[k+3]);
	}
}

int result(double *v){
	printf("El resultado fue: %f\n",v[0]);
}

int main(int argc,char*argv[]){
	double *A;
	int i;
	int j;
	double timetick;

	//Controla los argumentos al programa
	if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
 	{
   		printf("\nUsar: %s n\n  n: Dimension del vector\n", argv[0]);
  		exit(1);
 	}

 	A=(double*)malloc(sizeof(double)*N);

	//Inicializacion del vector con los valores de N hasta 1
	for(i=0;i<N;i++){
		A[i] = N-i;
	}

	double limit = log2(N);
	int next = 1;
	int salto = 2;

	timetick = dwalltime();

	//Aca se viene el humo cosmico
	for(i=0;i<limit;i++){
		for(j=0;j<=N-salto;j=j+salto){
			A[j]=A[j]/A[j+next];
		}
		next=salto;
		salto=salto*2;
	}

	printf("Tiempo en segundos %f\n", dwalltime() - timetick);

	result(A);

	free(A);

	return(0);
}
