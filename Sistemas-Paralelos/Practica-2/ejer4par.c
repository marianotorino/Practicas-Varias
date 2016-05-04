#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//Dimension por defecto de las matrices
int N=100;

//Cantidad de threads por defecto
int T=2;

//Vector de numeros
int *v, *b;

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

void *sortear(void *s){
  int id=(int *) s;
  int inicio=*id*N/T;
  int fin=inicio+(N/T);
  int rght, rend, left;
  int i,k,j,m,t;

  for (k=inicio+1; k < fin; k *= 2 ) {       
    for (left=inicio; left+k < fin; left += k*2 ) {
       rght = left + k;        
            rend = rght + k;
            if (rend > fin) rend = fin; 
            m = left; i = left; j = rght; 
            while (i < rght && j < rend) { 
                if (v[i] <= v[j]) {         
                    b[m] = v[i]; i++;
                } else {
                    b[m] = v[j]; j++;
                }
                m++;
            }
            while (i < rght) { 
                b[m]=v[i]; 
                i++; m++;
            }
            while (j < rend) { 
                b[m]=v[j]; 
                j++; m++;
            }
            for (m=left; m < rend; m++) { 
                v[m] = b[m]; 
            }
        }
    }
    
  //Avisar que termine
  if(id % 2 == 0){
    //Espero por id+x
    int x,h;
    for(x=1;x<T;x*=2){
      //Si en esta etapa tengo que ordenar
      ult=(fin-inicio)*2;
      indx1 = inicio;
      indx2 = fin+1;//*etapa
      while(indx1 < fin && indx2 < ult){
        if(v[h]<v[])
      }
    }
  }

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

pthread_t p_threads[T];
pthread_attr_t attr;
pthread_attr_init (&attr);

int id[T];

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
    v[i]=N-i;
  }   

 //Realiza la busqueda

  timetick = dwalltime();
  
  for(i=0;i<T;i++){
    id[i]=i;
    pthread_create(&p_threads[i], &attr, sortear, (void *) &id[i]);
  }
  for(i=0;i<T;i++){
    pthread_join(p_threads[i],NULL);
  }

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  for (h=0;h<N;h++){
    if(v[h]>v[h+1]) check=1;
  }

  if(check) printf("El resultado fue correcto!!!\n");
  else printf("El resultado fue erroneo\n");

 return(0);
}
