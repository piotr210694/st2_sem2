#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define NLOOPS 100000000

double f(double x)
{
	//return 3.0 * x * x; //1.0
	return 4.0 / (1.0 + x * x); //3.14....
}

int main(int argc, char *argv[])
{

    int id, p;
    long int i, n, nsum, np;
    double x, y, z, V, a, b, c;
    double tStart, tStop;
    double zsum, calka;

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
	
    z = 0.0;
    for (i = 0; i < np; i++)
    {
        x = (double) rand() / (double) RAND_MAX;
		z = z + f(x);

        double helper = (x * x) / (a * a) + (y * y) / (b * b) + (z * z) / (c * c);

        if (helper <= 1.0)
            n++;
    }

    MPI_Allreduce(&z, &zsum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    tStop = MPI_Wtime();

    //V = (double) nsum / NLOOPS * 8.0 * a * b * c;
    calka = zsum / NLOOPS;

    if (id == 0)
    {
        printf("Loops = %d Wynik V = %f\n", NLOOPS, calka);
        printf("Czas = %f s\n", tStop - tStart);
    }

    MPI_Finalize();
    return 0;
}
