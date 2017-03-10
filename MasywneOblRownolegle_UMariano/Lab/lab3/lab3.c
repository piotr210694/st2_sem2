#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define NLOOPS = 100000000

int main(int argc, char *argv[])
{
    int id, p;
    int i, n, nsum, np;
    double x, y, z, wyn;
    double tStart, tStop;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id)
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    srand(time(NULL));
    
    MPI_Barrier(MPI_COMM_WORLD);
    tStart = MPI_Wtime();
    n = 0;
    

    return 0;
}