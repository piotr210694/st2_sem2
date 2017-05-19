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
    int size = 1;
    int id, p, i;
    int sendDane[size];
    int * recvDane;
    int root = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    for (i = 0; i < size; i++)
    {
        sendDane[i] = id + 1;
    }
    recvDane = (int *)malloc(sizeof(int) * size * p);
    for (i = 0; i < p * size; i++)
    {
        recvDane[i] = 0;
    }

    wydruk(id, p, sendDane, size);
    MPI_Gather(
        sendDane, 
        size, 
        MPI_INT,
        recvDane,
        size,
        MPI_INT,
        root,
        MPI_COMM_WORLD);
    wydruk(id, p, recvDane, size * p);

    free(recvDane);
    MPI_Finalize();

    return 0;
}