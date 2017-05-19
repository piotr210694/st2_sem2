#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 10

int main(int argc, char * argv[])
{
    int id, p, i, j;
    float a[N];
    float b[N];
    int k, w;
    int ind = 0;
    MPI_Comm komWiersza;
    MPI_Comm komKolumny;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if ((p != N * N) && (id == 0))
    {
        printf("Wymagane %d procesow.\n", N * N);
        return 1;
    }

    if (id == 0)
    {
        srand(time(NULL));
        for (i = 0; i < N; i++)
        {
            a[i] = (float)rand() / (float)RAND_MAX;
            b[i] = 0;
        }
    }
    MPI_Bcast(a, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
    i = id / N;
    j = id % N;
    
    if (a[i] > a[j])
        w = 1;
    else
        w = 0;

    MPI_Comm_split(
        MPI_COMM_WORLD,
        i,
        j,
        &komWiersza
    );
    
    MPI_Reduce(
        &w,
        &ind,
        1,
        MPI_INT,
        MPI_SUM,
        0,
        komWiersza
    );

    MPI_Comm_split(
        MPI_COMM_WORLD,
        j,
        ind,
        &komKolumny
    );

    if (j == 0)
    {
        MPI_Gather(
            &a[i],
            1,
            MPI_FLOAT,
            b,
            1,
            MPI_FLOAT,
            0,
            komKolumny
        );
    }

    if ((j == 0) && (ind == 0))
    {
        printf("a:\n");
        for (k = 0; k < N; k++)
        {
            printf("%f ", a[k]);
        }
        printf("\nb:\n");
        for (k = 0; k < N; k++)
        {
            printf("%f ", b[k]);
        }
        printf("\n");
    }

    MPI_Comm_free(&komWiersza);
    MPI_Comm_free(&komKolumny);
    MPI_Finalize();

    return 0;
}