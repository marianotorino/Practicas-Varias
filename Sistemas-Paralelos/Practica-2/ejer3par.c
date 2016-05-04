#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de threads por defecto
int T=2;

//Declaracion del semaforo
pthread_mutex_t mutexMin, mutexMax;

//Vector de numeros
int *v;
int min_min;
int max_max;

//Variable compartida
int ocurrencias=0;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void *buscar(void *s){
    int i;
    int max = -1;
    int min = 999999;
    int inicio=*((int *) s)*N/T;
    int fin=inicio+(N/T);
    for(i=inicio;i<fin;i++){
      if(v[i] < min){
        min = v[i];
      }
      if(v[i] > max){
        max = v[i];
      }
    }

    pthread_mutex_lock(&mutexMin);
    if(min < min_min){
      min_min = min; 
    }
    pthread_mutex_unlock(&mutexMin);

    pthread_mutex_lock(&mutexMax);
    if(max > max_max){
      max_max = max; 
    }
    pthread_mutex_unlock(&mutexMax);
}


int main(int argc,char*argv[]){
 int i;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0))
  {
    printf("\nUsar: %s n t\n  n: Dimension del vector (nxn X nxn)\n  t: Cantidad de threads\n", argv[0]);
    exit(1);
  }

min_min = 999999;
max_max = -1;

v=malloc(sizeof(int)*N);

pthread_mutex_init(&mutexMin, NULL); // Inicializamos el mutex
pthread_mutex_init(&mutexMax, NULL);

pthread_t p_threads[T];
pthread_attr_t attr;
pthread_attr_init (&attr);

int id[T];

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
    v[i]=i;
  }   

 //Realiza la busqueda

  timetick = dwalltime();
  
  for(i=0;i<T;i++){
    id[i]=i;
    pthread_create(&p_threads[i], &attr, buscar, (void *) &id[i]);
  }
  for(i=0;i<T;i++){
    pthread_join(p_threads[i],NULL);
  }

  printf("Minimo: %d\nMaximo: %d\n", min_min, max_max);

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 return(0);
}
