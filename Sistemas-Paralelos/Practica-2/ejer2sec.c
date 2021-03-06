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
 int x,i;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 4) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[3])) <= 0))
  {
    printf("\nUsar: %s n t\n  n: Dimension de la matriz (nxn X nxn)\n  t: Cantidad de threads\n", argv[0]);
    exit(1);
  }

x=atoi(argv[2]);

int v[N];
int ocurrencias=0;

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
    scanf("%d",&v[i]);
  }   

 //Realiza la busqueda

  timetick = dwalltime();
  
  for(i=0;i<N;i++){
    if(v[i] == x){
      ocurrencias++;
    }
  }

  printf("Ocurrencias: %d\n",ocurrencias);

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 return(0);
}
