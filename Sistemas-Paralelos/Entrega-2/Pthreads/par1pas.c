#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de threads por defecto
int T=4;

//Cantidad de matrices
#define m 4

typedef struct { 
  pthread_mutex_t count_lock; 
  pthread_cond_t ok_to_proceed; 
  int count;
} barrier;

double escalar;
barrier *barrera;
double max, min, suma;
double *R;
double* M[m];
pthread_mutex_t mutexMax;
pthread_mutex_t mutexMin;
pthread_mutex_t mutexAvg;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

void *hilar(void *s){
    int i,k,j,c;
    int tid = *((int*)s);
    int inicio=tid*(N/T);
    int fin=inicio+N/T;
    double minLocal, maxLocal, sumaLocal;
    minLocal=9999;
    maxLocal=-999;
    sumaLocal=0;

    //multiplicar x filas

  for(i=inicio;i<fin;++i){
    for(j=0;j<N;++j){
      R[i*N+j]=0;
      if(M[0][i*N+j] < minLocal) minLocal=M[0][i*N+j];
      if(M[0][i*N+j] > maxLocal) maxLocal=M[0][i*N+j];
      if(M[1][i*N+j] < minLocal) minLocal=M[1][i*N+j];
      if(M[1][i*N+j] > maxLocal) maxLocal=M[1][i*N+j];
      sumaLocal+=M[0][i*N+j]+M[1][i*N+j];
      for(c=0;c<N;++c){
        R[i*N+j]+=M[0][i*N+c]*M[1][c+j*N];
      }
    }
  }

    for(k=2;k<m;++k){
      if(k%2==1){
        for(i=inicio;i<fin;++i){
          for(j=0;j<N;++j){
            R[i*N+j]=0;
            if(M[k][i*N+j] < minLocal) minLocal=M[k][i*N+j];
            if(M[k][i*N+j] > maxLocal) maxLocal=M[k][i*N+j];
            sumaLocal+=M[k][i*N+j];
            for(c=0;c<N;++c){
              R[i*N+j]+=M[0][i*N+c]*M[k][c+j*N];
            }
          }
        }
      } else {
        for(i=inicio;i<fin;++i){
          for(j=0;j<N;++j){
            M[0][i*N+j]=0;
            if(M[k][i*N+j] < minLocal) minLocal=M[k][i*N+j];
            if(M[k][i*N+j] > maxLocal) maxLocal=M[k][i*N+j];
            sumaLocal+=M[k][i*N+j];
            for(c=0;c<N;++c){
              M[0][i*N+j]+=R[i*N+c]*M[k][c+j*N];
            }
          }
        }
      }
    }

    pthread_mutex_lock(&mutexMax);
    if(maxLocal>max)    
      max=maxLocal;
    pthread_mutex_unlock(&mutexMax);
    pthread_mutex_lock(&mutexMin);
    if(minLocal<min)
      min=minLocal;
    pthread_mutex_unlock(&mutexMin);
    pthread_mutex_lock(&mutexAvg);
    suma+=sumaLocal;
    pthread_mutex_unlock(&mutexAvg);

    pthread_mutex_lock(&(barrera -> count_lock));
    barrera -> count ++;
    if (barrera -> count == T){
      barrera -> count = 0;
      escalar=pow(((max-min)/(suma/(N*N*m))),m);
      if(m%2 == 1){
        free(R);
        R=M[0];
      }
      pthread_cond_broadcast(&(barrera -> ok_to_proceed));
    }else{
      pthread_cond_wait(&(barrera -> ok_to_proceed),&(barrera -> count_lock));
    }
    pthread_mutex_unlock(&(barrera -> count_lock));



  for(i=inicio;i<fin;++i){
    for(j=0;j<N;++j){
      R[i*N+j] *= escalar;
    }
  }

    ////termina
}

int main(int argc,char*argv[]){
 int i,j,k;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0))
  {
    printf("\nUsar: %s n t\n  n: Dimension de la matriz (nxn X nxn)\n  t: Cantidad de threads\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  R=(double*)malloc(sizeof(double)*N*N);

  for(i=0;i<m;i++){
    M[i]=(double*)malloc(sizeof(double)*N*N);
  }
 //Inicializa las matrices
 for(k=0;k<m;++k){
   for(i=0;i<N;++i){
     for(j=0;j<N;++j){
       M[k][i*N+j] = (double) (rand() % 1000000000);
     }
   }
 }

  pthread_t p_threads[T];
  pthread_attr_t attr;
  pthread_attr_init (&attr);
  int id[T];
  
  pthread_mutex_init(&mutexMax, NULL); 
  pthread_mutex_init(&mutexMin, NULL); 
  pthread_mutex_init(&mutexAvg, NULL); 

  barrera = (barrier*)malloc(sizeof(barrier));
  barrera -> count = 0; 
  pthread_mutex_init(&(barrera -> count_lock), NULL); 
  pthread_cond_init(&(barrera -> ok_to_proceed), NULL);


  min=9999;
  max=-999;
  suma=0;
 //Realiza el calculo
  timetick = dwalltime();
  
  for(i=0;i<T;i++){
    id[i]=i;
    pthread_create(&p_threads[i], &attr, hilar, (void *) &id[i]);
  }
  //pthread_mutex_lock(&fin);
  for(i=0;i<T;i++){
    pthread_join(p_threads[i],NULL);
  }
 printf("\nTiempo en segundos %f\n", dwalltime() - timetick);

  free(barrera);
  free(R);
  for(i=1;i<m;i++){
    free(M[i]);
  }
 return(0);
}
