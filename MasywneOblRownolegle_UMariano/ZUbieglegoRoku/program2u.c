#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// kompilacja warunkowa
#ifndef _OPENMP
 #define omp_get_wtime()=0
#endif


int
main(int argc, char *argv[])
{
    double **A, **B, **C;
    long rows, columns;
    clock_t begin_t, end_t;
    double start, stop;
    long i, j, k;

    rows=atoi(argv[1]);
    
    columns=rows;

    // Inicjujemy 3 macierze
    
    A = (double**)malloc(rows * sizeof(double*));
    B = (double**)malloc(rows * sizeof(double*));
    C = (double**)malloc(rows * sizeof(double*));
    
    for (i = 0; i < rows; i++){
        A[i] = (double*)malloc(columns * sizeof(double));
        B[i] = (double*)malloc(columns * sizeof(double));
        C[i] = (double*)malloc(columns * sizeof(double));
        }
        
// Wypelniamy macierze A i B dowolnymi wartosciami

    for (i = 0; i < columns; i++)
    for (j = 0; j < rows; j++ ){
            A[j][i] = (double)(i * j / 100.0f);
            B[j][i] = (double)(i * j / 1000.0f);
        }    

// Testujemy czas obliczen dla roznych kolejnosci petli
//////////////////////////////////////////////////////////////////////////////////////
// zwykle mierzenie czasu 

    for (i = 0; i < columns; i++)
    for ( j = 0; j < rows; j++ )
            C[j][i] = 0.0f;

    begin_t = clock();

    for (i = 0; i < rows; i++)
    for (j = 0; j < rows; j++) 
    for (k = 0; k < columns; k++)
        C[i][j]  += A[i][k] * B[k][j];

    end_t = clock();

    printf("Czas obliczen IJK: %f.\n", ((double) (end_t - begin_t))/CLOCKS_PER_SEC);


 // czas na dwóch wątkach

   
    for (i = 0; i < columns; i++)
    for ( j = 0; j < rows; j++ )
            C[j][i] = 0.0f;

    start = omp_get_wtime();
    #pragma omp parallel for shared(A,B,C,rows,columns) private(i,j,k)
    for (i = 0; i < rows; i++)
    for (j = 0; j < rows; j++) 
    for (k = 0; k < columns; k++)
        C[i][j]  += A[i][k] * B[k][j];

    stop = omp_get_wtime();

    printf("Czas obliczen IJK OMP: %f.\n", stop-start);
////////////////////////////////////////////////////////////////////

    for (i = 0; i < columns; i++)
    for ( j = 0; j < rows; j++ )
            C[j][i] = 0.0f;

    begin_t = clock();

    for (i = 0; i < rows; i++)
    for (k = 0; k < columns; k++)
    for (j = 0; j < rows; j++) 
        C[i][j]  += A[i][k] * B[k][j];

    end_t = clock();

    printf("Czas obliczen IKJ: %f.\n", ((double) (end_t - begin_t))/CLOCKS_PER_SEC);

//////////////////
    for (i = 0; i < columns; i++)
    for ( j = 0; j < rows; j++ )
            C[j][i] = 0.0f;

    begin_t = clock();

    for (j = 0; j < rows; j++) 
    for (i = 0; i < rows; i++)
    for (k = 0; k < columns; k++)
        C[i][j]  += A[i][k] * B[k][j];

    end_t = clock();

    printf("Czas obliczen JIK : %f.\n", ((double) (end_t - begin_t))/CLOCKS_PER_SEC);

//////////////

    for (i = 0; i < columns; i++)
    for ( j = 0; j < rows; j++ )
            C[j][i] = 0.0f;

    begin_t = clock();

    for (j = 0; j < rows; j++) 
    for (k = 0; k < columns; k++)
    for (i = 0; i < rows; i++)
        C[i][j]  += A[i][k] * B[k][j];

    end_t = clock();

    printf("Czas obliczen JKI : %f.\n", ((double) (end_t - begin_t))/CLOCKS_PER_SEC);

///////////////

    for (i = 0; i < columns; i++)
    for ( j = 0; j < rows; j++ )
            C[j][i] = 0.0f;

    begin_t = clock();

    for (k = 0; k < columns; k++)
    for (i = 0; i < rows; i++)
    for (j = 0; j < rows; j++) 
        C[i][j]  += A[i][k] * B[k][j];

    end_t = clock();

    printf("Czas obliczen KIJ : %f.\n", ((double) (end_t - begin_t))/CLOCKS_PER_SEC);
/////////////


    for (i = 0; i < columns; i++)
    for ( j = 0; j < rows; j++ )
            C[j][i] = 0.0f;

    begin_t = clock();

    for (k = 0; k < columns; k++)
    for (j = 0; j < rows; j++) 
    for (i = 0; i < rows; i++)
        C[i][j]  += A[i][k] * B[k][j];

    end_t = clock();

    printf("Czas obliczen KJI : %f.\n", ((double) (end_t - begin_t))/CLOCKS_PER_SEC);



// Zwalniamy pamięć.

    for (i = 0; i < rows; i++){
        free(A[i]);
        free(B[i]);
        free(C[i]);
        }

    free(A);
    free(B);
    free(C);

    return 0;
}
