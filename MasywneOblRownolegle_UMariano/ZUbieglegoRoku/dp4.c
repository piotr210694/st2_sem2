// kompilacja:  gcc -o1 -msse -fopenmp program.c -o program -lm 
// uruchomienie: env OMP_NUM_THREADS=2 ./program 1000

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    double **A, *b, *x;
    double c;

    double begin_to, end_to;
    long i, j, k, l, n;
    srand(time(NULL));
    n = atoi(argv[1]);
    A = (double **) malloc((n + 1) * sizeof (double*));
    b = (double *) malloc((n + 1) * sizeof (double));
    x = (double *) malloc((n + 1) * sizeof (double));

    for (i = 1; i <= n; i++)
        A[i] = (double*) malloc((n + 1) * sizeof (double));


    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            A[i][j] = (rand() % 9) + 1;
            //printf("%.0f ",A[i][j]);
        }
        b[i] = (rand() % 12) + 1;
        //printf("| %.0f\n",b[i]);
    }

    begin_to = omp_get_wtime();

    for (k = 1; k <= n; k++) {
#pragma omp parallel for shared(A,b,k,n) private(c,i,j)
        for (i = 1; i <= n; i++) {
            if (i == k) {
                continue;
            }
            c = A[i][k] / A[k][k];
            for (j = k; j <= n; j++) {
                A[i][j] = A[i][j] - c * A[k][j];
            }
            b[i] = b[i] - c * b[k];
        }
    }

    for (l = 1; l <= n; l++)
        x[l] = b[l] / A[l][l];

    x[n] = b[n] / A[n][n];
    for (k = n - 1; k >= 1; k--) {
        x[k] = b[k];
        for (j = k + 1; j <= n; j++) {
            x[k] = x[k] - A[k][j] * x[j];
        }
        x[k] = x[k] / A[k][k];
    }

    end_to = omp_get_wtime();

    printf("Czas obliczen openMP  %f sek \n", end_to - begin_to);
    //printf("Rozwiazania: x[1], x[2] = %f, %f\n", x[1], x[2]);
    return 0;
}
