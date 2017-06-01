#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{
    double **A;
    double *b, *x;
    double c;
    double t1, t2;
    long int i, j, k, l, n;
    n = atol(argv[1]);
    A = (double **)malloc((n + 1) * sizeof(double));
    b = (double *)malloc((n + 1) * sizeof(double));
    x = (double *)malloc((n + 1) * sizeof(double));

    for (i = 1; i < n + 1; i++)
        A[i] = (double *)malloc((n + 1) * sizeof(double));

    // inicjalizacja tablic A i B
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <=n; j++)
        {
            A[j][i] = (double)(i + j) / 1000.0;
        }
        b[i] = (double)(i / 1000.0);
    }

    A[1][1] = 1;
    A[1][2] = 2;
    A[2][1] = 3;
    A[2][2] = 4;
    b[1] = 5;
    b[2] = 6;

    t1 = omp_get_wtime();

    // alogrytm eliminacji Gaussa - etap eliminacji zmiennych
    //#pragma omp parallel for shared(A, b, n) private(i, j, k, c)
    //niepoprawne
    for (k = 1; k <= n - 1; k++)
    {
        #pragma omp parallel for shared(A, b, n, k) private(i, j, c)
        for (i = k + 1; i <= n; i++)
        {
            c = A[i][k] / A[k][k];
            //#pragma omp parallel for shared(A, b, k, i, c, n) private(j)
            for (j = k; j <= n; j++)
            {
                A[i][j] = A[i][j] - c * A[k][j];
            }

            b[i] = b[i] - c * b[k];
        }
    }

    // postępowanie odwrotne
    x[n] = b[n] / A[n][n];
    for (k = n - 1; k >= 1; k--)
    {
        x[k] = b[k];
        for (j = k + 1; j <= n; j++)
        {
            x[k] = x[k] - A[k][j] * x[j];
        }

        x[k] = x[k] / A[k][k];
    }

    t2 = omp_get_wtime();

    printf("Czas obliczen: %f\n", t2 - t1);
    printf("Rozwiązanie: x1, x2 = %f %f\n", x[1], x[2]);

    return 0;
}