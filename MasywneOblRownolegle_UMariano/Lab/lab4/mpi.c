#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define NLOOPS 100000000

int main(int argc, char *argv[])
{
    int id, p;
    long int i, n, nsum, np;
    double x, y, z, V, a, b, c;
    double tStart, tStop;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    srand(time(NULL));
    
    MPI_Barrier(MPI_COMM_WORLD);
    tStart = MPI_Wtime();

    n = 0;
    a = 1;
    b = 2;
    c = 5;
    np = NLOOPS / p;

    for (i = 0; i < np; i++)
    {
        x = (double) rand() / (double) RAND_MAX * a;
        y = (double) rand() / (double) RAND_MAX * b;
        z = (double) rand() / (double) RAND_MAX * c;

        double helper = (x * x) / (a * a) + (y * y) / (b * b) + (z * z) / (c * c);

        if (helper <= 1.0)
            n++;
    }

    MPI_Allreduce(&n, &nsum, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    tStop = MPI_Wtime();

    V = (double) nsum / NLOOPS * 8.0 * a * b * c;

    if (id == 0)
    {
        printf("Loops = %d Wynik V = %f\n", NLOOPS, V);
        printf("Czas = %f s\n", tStop - tStart);
    }

    MPI_Finalize();
    return 0;
}