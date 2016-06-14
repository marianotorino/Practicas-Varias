#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

void main(int argc,char*argv[]) {
  int id;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  int i,cod;
  int* entero;
  entero=(int*) malloc(sizeof(int));
  MPI_Status estado;
  if(id == 0) {
	*entero = 9;
	for(i=0;i<3;i++){
	    MPI_Recv(entero, sizeof(int), MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado);
	    printf("[%d]: variable i: %d\n",id,i);
	}
  } else {
	  *entero=8;
          printf("[%d]: enviando cod",id);
	  MPI_Send(entero, sizeof(int), MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
}
