// kompilacja
// gcc -o1 -msse -fopenmp program1.c -o program1 -lm
// uruchomienie
// ./program1 8000000
// env OMP_NUM_THREADS=2 ./program1 8000000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main( int argc, char *argv[] )
{

    double **A, *b, *x;
    double c;
    clock_t begin_t, end_t;
    double begin_to, end_to;
    long i, j, k, l, n;
    n=atoi(argv[1]); //n=2; chodzi o macierz n x n


//Inicjujemy macierze i wektory
    A=(double **)malloc((n+1)*sizeof(double*));
    b=(double*)malloc((n+1)*sizeof(double*));
    x=(double*)malloc((n+1)*sizeof(double*));

    for(i=1; i<=n; i++)
        A[i]=(double*)malloc((n+1)*sizeof(double*));
/*
    A[1][1]=1;
    A[1][2]=2;
    A[2][1]=3;
    A[2][2]=4;
    b[1]=5;
    b[2]=6;
*/

    for (i=1; i<=n; i++){
	for (j=1; j<=n; j++){
	   A[j][i] = (double) ((i+j) / 1000.f);
	   b[i] = (double) (i / 1000.f);
		}
	}

//ETAP ELIMINACJI ZMIENNYCH
//petla wierszy eliminujacych (kolumn eliminowanych)

    begin_t = clock(); 
    begin_to = omp_get_wtime();

    for(k=1; k<=n-1; k++)
    {
//petla modyfikacji wierszy ponizej k-tego wiersza
	#pragma omp parallel for shared (A,b,k,n) private (i,j,c)
        for(i=k+1; i<=n; i++)
        {
            c=A[i][k]/A[k][k];
//petla kolumn w drugim wierszu
            for(j=k; j<=n; j++)
            {
                A[i][j]=A[i][j]-c*A[k][j];
            }
            b[i]=b[i]-c*b[k];
        }
    }
    
    end_t = clock();
    end_to = omp_get_wtime();

	printf("Czas obliczen 'omp': %f \n", (double) (end_to - begin_to));
	printf("Czas obliczen: %f \n", (double)  (end_t - begin_t)/(CLOCKS_PER_SEC)) ;


//POSTEPOWANIE ODWROTNE
    x[n]=b[n]/A[n][n];
//petla wierszy od przedostaniego do pierwszego
    for(k=n-1; k>=1; k--)
    {
        x[k]=b[k];
//petla juz obliczonych zmiemnnych
        for(j=k+1; j<=n; j++)
        {
            x[k]=x[k]-A[k][j]*x[j];
        }
        x[k]=x[k]/A[k][k];
    }

    printf("Rozwiazania: x[1], x[2] = %f, %f\n", x[1], x[2]);

    

    return 0;
}
