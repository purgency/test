#include "mpi.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>


int main (int argc, char *argv[])
{
    char hostname[30];
    gethostname(hostname, 29);
    struct timeval time, endtime;
    gettimeofday(&time,NULL);
    int rank, size;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    if(rank == 0){
        int i;
        for(i = 0; i < size; i++){
            MPI_Status status;
            char string[99];
            MPI_Recv(string,99,MPI_CHAR,i,0,MPI_COMM_WORLD,&status);
            printf(string);
        }
    } else {
        char string[99];
        sprintf(string,"%s : %ld\n",hostname,time.tv_usec);
        MPI_Send(string,99,MPI_CHAR,0,0,MPI_COMM_WORLD);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    gettimeofday(&endtime, NULL);
    printf("Rang %d beendet jetzt %ld\n", rank, endtime.tv_usec);
    MPI_Finalize();
    return 0;
}
