#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int*
init (int N)
{
	// TODO
	int* buf = malloc(sizeof(int) * N);

	srand(time(NULL));

	for (int i = 0; i < N; i++)
	{
		// Do not modify "% 25"
		buf[i] = rand() % 25;
	}

	return buf;
}

int*
circle (int* buf,int* rank,MPI_Comm_rank* MPI_COMM_WORLD,int* size)
{
    MPI_Status status;
    
    int predecessor;
    int successor;
    
    if(*rank == 0){
        predecessor = size;
        successor = 1;
    } else if(*rank == size) {
        predecessor = *rank - 1;
        successor = 0;
    } else {
        predecessor = *rank - 1;
        successor = *rank + 1;
    }
    
    if(*rank % 2 == 0){
        MPI_Recv(todo,todo,todo,predecessor,0,*MPI_COMM_WORLD,&status);
        MPI_Send(todo,todo,todo,successor,0,*MPI_COMM_WORLD);
    } else {
        MPI_Send(todo,todo,todo,successor,0,*MPI_COMM_WORLD);
        MPI_Recv(todo,todo,todo,predecessor,0,*MPI_COMM_WORLD,&status);
    }
    
	
    
    // TODO
	return buf;
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
    
	// Array length
	N = atoi(argv[1]);
	buf = init(N);

	MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    
    //division kinda sucks because it might be some decimal which will fail to be represented by int
    int* bufpart = buf + (rank-1) * (N / size);

	printf("\nBEFORE\n");

	for (int i = 0; i < N; i++)
	{
		printf("rank %d: %d\n", rank, buf[i]);
	}

	circle(bufpart,&rank, &MPI_COMM_WORLD, &size);

	printf("\nAFTER\n");

	for (int j = 0; j < N; j++)
	{
		printf("rank %d: %d\n", rank, buf[j]);
	}
    
    MPI_Finalize();
    
    free(buf);

	return EXIT_SUCCESS;
}