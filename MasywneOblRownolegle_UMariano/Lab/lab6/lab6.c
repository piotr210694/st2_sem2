#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);

    float *a = (float*)malloc(N * sizeof(float));
    float *b = (float*)malloc(N * sizeof(float));
    int *ind = (int*)malloc(N * sizeof(int));

    int i,j;
    double t1, t2;

    srand(time(NULL));
    for (i = 0; i < N; i++)
    {
        a[i] = (float)rand() / RAND_MAX;
    }

    t1 = omp_get_wtime();

    #pragma omp parallel for private(i) shared(a, b, ind) 
    for (i = 0; i < N; i++)
    {
        ind[i] = 0;

        #pragma omp parallel for private(j)
        for (j = 0; j < N; j++)
        {
            if ((a[i] > a[j]) || ((a[i] == a[j]) && (i > j)))
            {
                #pragma omp critical
                ind[i]++;
            }
        }

        b[ind[i]] = a[i];
    }
    t2 = omp_get_wtime();

    printf("a:\n");
    for (i = 0; i < 10; i++)
    {
        printf("%6.1f ", a[i]);
    }

    printf("\nb:\n");
    for (i = 0; i < 10; i++)
    {
        printf("%6.1f ", b[i]);
    }
    printf("\nCzas: %6.1f\n", t2 - t1);

    free(a);
    free(b);
    free(ind);
    return 0;
}