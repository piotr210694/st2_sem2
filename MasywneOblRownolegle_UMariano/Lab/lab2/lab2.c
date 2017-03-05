#include <stdio.h>
#include <time.h>
#include <omp.h>

int main(int arg, char *argv[])
{
    double **A, **B, **C;
    long N, i, j, k;
    clock_t t1, t2;
    double tstart, tstop;

    N = atol(argv[1]);

    // alokacja tablic
    A = (double**)malloc(N * sizeof(double*));
    B = (double**)malloc(N * sizeof(double*));
    C = (double**)malloc(N * sizeof(double*));

    for (i = 0; i < N; i++)
    {
        A[i] = (double*)malloc(N * sizeof(double));
        B[i] = (double*)malloc(N * sizeof(double));
        C[i] = (double*)malloc(N * sizeof(double));
    }

    // wypełnianie tablic wartościami
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            A[j][i] = (double)(i * j) / 10.0;
            B[j][i] = (double)(2 * i + 3 * j) / 20.0;
        }
    }

    // zerowanie elementów tablicy C
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            C[i][j] = 0.0;
        }
    }

    // algorytm
    //t1 = clock();
    tstart = omp_get_wtime();
    #pragma omp parallel for shared(A,B,C,N) private(i,j,k)
    for (i = 0; i < N; i++)
    {
        for (k = 0; k < N; k++)
        {
            for (j = 0; j < N; j++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    //t2 = clock();
    tstop = omp_get_wtime();

    //printf("Czas obliczeń ijk: %f\n", ((double)(t2 - t1)) / CLOCKS_PER_SEC);
    printf("Czas obliczeń OPEN ijk: %f\n", tstop - tstart);

    return 0;
}
