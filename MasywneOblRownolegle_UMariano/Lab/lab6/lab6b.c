#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

void wydruk(int id, int p, int *dane, int rozmiar)
{
    int i, n;
    for (n = 0; n < p; n++)
    {
        if (n == id)
        {
            printf(" %d: ", id);
            for (i = 0; i < rozmiar; i++)
            {
                printf(" %d", dane[i]);
            }

            printf("\n");

            if (id == p - 1)
            {
                printf("\n");
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int main(int argc, char * argv[])
{
    int id, p, i;
    int sendDane[5];
    int * recvDane;
    int root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    for (i = 0; i < 5; i++)
    {
        sendDane[i] = id + 1;
    }
    recvDane = (int *)malloc(sizeof(int) * 5 * p);
    for (i = 0; i < p * 5; i++)
    {
        recvDane[i] = 0;
    }

    wydruk(id, p, sendDane, 5);
    MPI_Gather(
        sendDane, 
        5, 
        MPI_INT,
        recvDane,
        5,
        MPI_INT,
        root,
        MPI_COMM_WORLD);
    wydruk(id, p, recvDane, 5 * p);

    free(recvDane);
    MPI_Finalize();

    return 0;
}