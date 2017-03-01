//METODA ELIMINACJI GAUSSA - JORDANA
//zagdnienie: rozwiazac uklad rownan liniowych A*x=b

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main( int argc, char *argv[] )
{

    double **A, *b, *x;
    double c;
//double clock_t begin_t, end_t;
//double begin_to, end_to;
    double start, stop;
    double start_o, stop_o;
    long i, j, k, l, n;
    n=atoi(argv[1]);
//n=2;

//Inicjujemy macierze i wektory
    A=(double **)malloc((n+1)*sizeof(double*));
    b=(double*)malloc((n+1)*sizeof(double*));
    x=(double*)malloc((n+1)*sizeof(double*));

    for(i=1; i<=n; i++)
        A[i]=(double*)malloc((n+1)*sizeof(double*));

    /*A[1][1]=1;
    A[1][2]=2;
    A[2][1]=3;
    A[2][2]=4;
    b[1]=5;
    b[2]=6;*/

//ETAP ELIMINACJI ZMIENNYCH
//petla wierszy eliminujacych (kolumn eliminowanych)
    start=clock();
    start_o=omp_get_wtime();
    for(k=1; k<=n; k++)
    {
        #pragma omp parallel for shared (A, b, k, n) private (i, j, c)
//petla modyfikacji wierszy ponizej k-tego wiersza
        for(i=1; i<=n; i++)
        {
            if(i==k)
            {
                continue;
            }
            c=A[i][k]/A[k][k];
//petla kolumn w drugim wierszu
            for(j=k; j<=n; j++)
            {
                A[i][j]=A[i][j]-c*A[k][j];
            }
            b[i]=b[i]-c*b[k];
        }
    }
    for(l=1; l<=n; l++)
    {
        x[l]=b[l]/A[l][l];
    }


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
    stop=clock();
    stop_o=omp_get_wtime();

    printf("Rozwiazania: x[1], x[2] = %f %f\n", x[1], x[2]);
    printf("czas obliczen sekwencyjne: %f.\n",(double)(stop-start)/CLOCKS_PER_SEC);
    printf("czas obliczen OMP: %f.\n",stop_o-start_o);

    return 0;
}
