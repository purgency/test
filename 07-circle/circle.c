#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int*
init (int N,int size)
{
	// TODO
	int* buf = malloc(sizeof(int) * (N / size));

	srand(time(NULL));

	for (int i = 0; i < N / size; i++)
	{
		// Do not modify "% 25"
		buf[i] = rand() % 25;
	}

	return buf;
}

int
circle (int* buf,int* rank,int* size, int* N)
{
    MPI_Status status;
    
    int predecessor;
    int successor;
    
    if(*rank == 0){
        predecessor = *size-1;
        successor = 1;
    } else if(*rank == *size-1) {
        predecessor = *rank - 1;
        successor = 0;
    } else {
        predecessor = *rank - 1;
        successor = *rank + 1;
    }
    
    int firstelem;
    
    if(*rank == 0){
        MPI_Send(buf[1],sizeof(int),MPI_INT,*size-1,0,MPI_COMM_WORLD);
    } else if(*rank == *size-1) {
        MPI_Recv(firstelem,sizeof(int),MPI_INT,0,0,MPI_COMM_WORLD,&status);
    }
    
    int boolean = 1;
    int terminator = 0;
            
    
    while(boolean){
        if(*rank % 2 == 0){
            int i;
            for(i = 0; i < N / size ;i++){
                MPI_Send(buf[i],sizeof(int),MPI_INT,successor,i,MPI_COMM_WORLD);
                MPI_Recv(buf[i],sizeof(int),MPI_INT,predecessor,i,MPI_COMM_WORLD,&status);
            }
        } else {
            int i;
            int bufcopy[N/size];
            for(i = 0; i < N / *size ;i++){
                bufcopy[i] = buf[i];
                MPI_Recv(buf[i],sizeof(int),MPI_INT,predecessor,i,MPI_COMM_WORLD,&status);
                MPI_Send(bufcopy[i],sizeof(int),MPI_INT,successor,i,MPI_COMM_WORLD);
            }
        }
        
        if(*rank == *size - 1){
            if(buf[1] == firstelem){
                terminator = 1;
            }
            int i;
            for(i = 0; i < *size ;i++){
                MPI_Send(terminator,sizeof(int),MPI_INT,i,0,MPI_COMM_WORLD);
            }
        } else {
            MPI_Recv(terminator,sizeof(int),MPI_INT,*size - 1,0,MPI_COMM_WORLD,&status);
        }
        
        if(terminator){
            boolean = 0;
        }
    }
    
    return 0;
}

int
main (int argc, char** argv)
{
	int N;
	int rank, size;
	int* buf;

	if (argc < 2)
	{
		printf("Arguments error!\nPlease specify a buffer size.\n");
		return EXIT_FAILURE;
	}
    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    
	// Array length
	N = atoi(argv[1]);
	buf = init(N, size);
    
    printf("\nBEFORE\n");

	for (int i = 0; i < N / size; i++)
	{
		printf("rank %d: %d\n", rank, buf[i]);
	}

	circle(buf,&rank, &size, &N);

	printf("\nAFTER\n");

	for (int j = 0; j < N / size; j++)
	{
		printf("rank %d: %d\n", rank, buf[j]);
	}
    
    free(buf);
    
    MPI_Finalize();

	return EXIT_SUCCESS;
}
