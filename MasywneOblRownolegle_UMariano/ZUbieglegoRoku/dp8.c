// kompilacja: mpicc  program.c -o program -lm
// uruchomienie: mpirun -n 1 ./program 1000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[]) {
    double **A, *b, *x, *xw;
    double c, czas;
    int i, j, k, l, n;
    int ib, ik, ni, kni;
    int id, p, korzen = 0;
    n = atoi(argv[1]);
    srand(time(NULL));

    // Inicjalizacja mpi
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Bcast(&n, 1, MPI_INT, korzen, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Inicjujemy macierze i wektory
    A = (double **) malloc((n + 1) * sizeof (double*));
    b = (double *) malloc((n + 1) * sizeof (double));
    x = (double *) malloc((n + 1) * sizeof (double));
    xw = (double *) malloc((n + 1) * sizeof (double));

    for (i = 1; i <= n; i++)
        A[i] = (double*) malloc((n + 1) * sizeof (double));

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            A[i][j] = (rand() % 9) + 1;
            if (id == 0)printf("%.0f ", A[i][j]);
        }
        b[i] = (rand() % 12) + 1;
        if (id == 0)printf("| %.0f\n", b[i]);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    czas = MPI_Wtime();

    ni = (int) (ceil((float) (n) / (float) (p)));
    ib = id * ni + 1;
    ik = ib + ni - 1;
    if (id == p - 1) {
        ik = n;
    }


    printf("%d \n", ni);
    // Etap eliminacji zmiennych - petla wierszy eliminujacych (kolumn eliminowanych)
    for (k = 1; k <= n; k++) {
        for (i = ib; i <= ik; i++) {
            if (i == k) {
                continue;
            }
            c = A[i][k] / A[k][k];

            for (j = k; j <= n; j++)
                A[i][j] = A[i][j] - c * A[k][j];
            b[i] = b[i] - c * b[k];
        }
        if (k < n) {
            kni = floor(k / ni);
            MPI_Bcast(A[k + 1], n + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            MPI_Bcast(&b[k + 1], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    }

    for (k = ib; k <= ik; k++)
        x[k] = b[k] / A[k][k];

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(&x[ib], ni, MPI_DOUBLE, &xw[ib], ni, MPI_DOUBLE, korzen, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (id == 0) {
        czas = MPI_Wtime() - czas;
        for (k = 1; k <= n; k++)
            printf("x%d: %f\n", k, xw[k]);
        printf("\nCzas obliczen  %f \n", czas);
    }

    for (i = 1; i <= n; i++)
        free(A[i]);

    free(A);
    free(b);
    free(x);
    free(xw);
    MPI_Finalize();

    return 0;
}
