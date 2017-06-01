#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#define IF for

int main(int argc, char** argv)
{
    double **A;
    double *b, *x;
    double c;
    double t1, t2;
    long int i, j, k, l, n;
    //n = atol(argv[1]);
    n = 2;
    A = (double **)malloc((n + 1) * sizeof(double));
    b = (double *)malloc((n + 1) * sizeof(double));
    x = (double *)malloc((n + 1) * sizeof(double));

    for (i = 1; i < n + 1; i++)
        A[i] = (double *)malloc((n + 1) * sizeof(double));

    A[1][1] = 1;
    A[1][2] = 2;
    A[2][1] = 3;
    A[2][2] = 4;
    b[1] = 5;
    b[2] = 6;

    // alogrytm eliminacji Gaussa - etap eliminacji zmiennych
    for (k = 1; k <= n - 1; k++)
    {
        for (i = k + 1; i <= n; i++)
        {
            c = A[i][k] / A[k][k];
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

    printf("Rozwiązanie: x1, x2 = %f %f\n", x[1], x[2]);

    return 0;
}