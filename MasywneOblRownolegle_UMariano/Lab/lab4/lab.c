#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include <mpi.h>

#define NLOOPS 100000000

int main(int argc, char *argv[])
{
    int id, p;
    long i, n, nsum, np;
    double x, y, z, wyn;
    double tStart, tStop;
    int a = 1, b = 2, c = 5;
    long N = NLOOPS;

    //MPI_Init(&argc, &argv);
   // MPI_Comm_rank(MPI_COMM_WORLD, &id)
    //MPI_Comm_size(MPI_COMM_WORLD, &p);

    srand(time(NULL));
    
    //MPI_Barrier(MPI_COMM_WORLD);
   // tStart = MPI_Wtime();
    n = 0;
    wyn = 0.0;
    
    for (i = 0; i < N; i++)
    {
        x = (double) rand() / (double) RAND_MAX * a;
        y = (double) rand() / (double) RAND_MAX * b;
        z = (double) rand() / (double) RAND_MAX * c;

        double helper = (x * x) / (a * a) + (y * y) / (b * b) + (z * z) / (c * c);

        if (helper <= 1.0)
            n++;
    }

    wyn = (double) n / N * 8.0 * a * b * c;
    printf("V = %f\n", wyn);

    return 0;
}