#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <mpi.h>

void wydruk(
    int id,
    int p,
    int *dane,
    int rozmiar)
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
    int id, p;
    int sendBuf[10] = { 0 };
    int *recvBuf;
    int stride;
    int *displs, *rCounts;
    int root, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    root = 0;
    stride = 12;

    for (i = 0; i < 10; i++)
        sendBuf[i] = i + id * 10 + 1;

    if (id == root)
    {
        recvBuf = (int*)malloc(p * stride * sizeof(int));
        for (i = 0; i < p * stride; i++)
            recvBuf[i] = 0;

        displs = (int*)malloc(p * sizeof(int));
        rCounts = (int*)malloc(p * sizeof(int));

        for (i = 0; i < p; i++)
        {
            displs[i] = i * stride;
            rCounts[i] = i + 1;
        }
    }

    wydruk(
        id,
        p,
        sendBuf,
        10
    );

    MPI_Gatherv(
        sendBuf,
        id + 1,
        MPI_INT,
        recvBuf,
        rCounts,
        displs,
        MPI_INT,
        root,
        MPI_COMM_WORLD
    );

    if (id == root)
    {
        printf(" %d: ", id);
        
        for (i = 0; i < p * stride; i++)
            printf(" %d ", recvBuf[i]);

        printf("\n");
    }

    if (id == 0)
    {
        free(recvBuf);
        free(displs);
        free(rCounts);
    }

    MPI_Finalize();
    return 0;
} 