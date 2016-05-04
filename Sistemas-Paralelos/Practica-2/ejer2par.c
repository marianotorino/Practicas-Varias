#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de threads por defecto
int T=2;

//Declaracion del semaforo
pthread_mutex_t mutex;

//Vector de numeros
int *v;
int x;

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
    int cant=0;
    int inicio=*((int *) s)*N/T;
    int fin=inicio+(N/T);
    for(i=inicio;i<fin;i++){
    if(v[i] == x){
    cant++;
  }
}

    pthread_mutex_lock(&mutex);
    ocurrencias += cant;
    pthread_mutex_unlock(&mutex);

}


int main(int argc,char*argv[]){
 int i;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 4) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[3])) <= 0))
  {
    printf("\nUsar: %s n t\n  n: Dimension de la matriz (nxn X nxn)\n  t: Cantidad de threads\n", argv[0]);
    exit(1);
  }

x=atoi(argv[2]);

v=malloc(sizeof(int)*N);

pthread_mutex_init(&mutex, NULL); // Inicializamos el mutex

pthread_t p_threads[T];
pthread_attr_t attr;
pthread_attr_init (&attr);

int id[T];

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
    v[i]=N;
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

  printf("Ocurrencias: %d\n",ocurrencias);

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 return(0);
}
