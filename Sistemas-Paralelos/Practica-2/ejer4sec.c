#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//Dimension por defecto de las matrices
int N=100;

int check = 0;
//Vector de numeros
int *v, *b;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}


int main(int argc,char*argv[]){
 int h;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0))
  {
    printf("\nUsar: %s n t\n  n: Dimension del vector (nxn X nxn)\n", argv[0]);
    exit(1);
  }

v=malloc(sizeof(int)*N);
b=malloc(sizeof(int)*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(h=0;h<N;h++){
    v[h]=N-h;
  }   

 //Realiza la busqueda

  timetick = dwalltime();
  
  int rght, rend, left;
  int i,k,j,m,t;

  for (k=1; k < N; k *= 2 ) {       
    for (left=0; left+k < N; left += k*2 ) {
       rght = left + k;        
            rend = rght + k;
            if (rend > N) rend = N; 
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

 printf("Tiempo en segundos %f\n", dwalltime() - timetick);

  
  for (h=0;h<N;h++){
    if(v[h]>v[h+1]) check=1;
  }

  if(check) printf("El resultado fue correcto!!!\n");
  else printf("El resultado fue erroneo\n");

 return(0);
}
