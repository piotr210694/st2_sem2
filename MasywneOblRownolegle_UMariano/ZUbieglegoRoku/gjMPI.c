//rozw. ukl. rownan liniowych met. Gaussa - Jordana z wyk. biblioteki MPI
//mpicc  Z.c -o Z -lm
//mpirun -n 1 ./Z 8

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <xmmintrin.h>
#include <mpi.h>


int main (int argc, char *argv[])
{
    double **A, *b, *x, *xw;
    double c, czas;
    int i, j, k, n;
    int ib, ik, ni, kni;
    int id, p, korzen = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(id == korzen)
    {
        printf("Podaj wymiar macierzy: ");
//        scanf("%id", &n);
	n=2;
    }
    MPI_Bcast(&n, 1, MPI_INT, korzen, MPI_COMM_WORLD);
//Inicjacja macierzy i wektorow
    A=(double**)malloc((n+1)*sizeof(double*));
    b=(double*)malloc((n+1)*sizeof(double*));
    x=(double*)malloc((n+1)*sizeof(double*));
    xw=(double*)malloc((n+1)*sizeof(double*));

    for(i=0; i<=n; i++)
        A[i]=(double*)malloc((n+1)*sizeof(double));
//wypelnainie tablic A i b wartosciami

    A[1][1]=1;
    A[1][2]=2;
    A[2][1]=3;
    A[2][2]=4;
    b[1]=5;
    b[2]=6;

//startujemy czas obliczen
    MPI_Barrier(MPI_COMM_WORLD);
    czas=MPI_Wtime();
//dzielimy caly zakres indeksow w petli 2-giej po i na podprzedzialy
    ni=(int)(ceil((float)(n)/(float)(p)));
//ni=n/p
    ib=id*ni+1;
    ik=ib+ni-1;
    if(id == p-1)
        ik=n;
//wydruk kontrolny
    printf("%d %d %d %d %d\n", n, p, ni, ib, ik);
//elgorytm eliminacji gaussa-Jordana
//petla wierszy eliminujacych
    for(k=1; k<=n; k++)
    {
//petla modyfikacji weiurszy powyzej i ponizej k-tego wiersza
        for(i=ib; i<=k; i++)
        {
            if(i==k)
                continue;
            c=A[i][k]/A[k][k];
//petla kolumn w danym wierszu
            for(j=k; j<n; j++)
                A[i][j]=A[i][j]-c*A[k][j];

            b[i]=b[i]-c*b[k];
        }
//rozlozenie wiersza eliminujacego
        if(k<n)
        {
            kni=k/ni;
            printf("%d\n", kni);
            MPI_Bcast(A[k+1], n+1, MPI_DOUBLE, kni, MPI_COMM_WORLD);
            MPI_Bcast(&b[k+1], 1, MPI_DOUBLE, kni, MPI_COMM_WORLD);
        }
    }
    for(k=ib; k<=ik; k++)
        x[k]=b[k]/A[k][k];
//wydruk kontrolny z podprzedzialow
    MPI_Barrier(MPI_COMM_WORLD);
    for(k=1; k<=n; k++)
        printf("%f \n", x[k]);
//zebranie wszytskich wynikow z podprzedzialow
    MPI_Gather(&x[ib], ni, MPI_DOUBLE, &xw[ib], ni, MPI_DOUBLE, korzen, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if(id == korzen)
    {
        printf("\n");
//wydruk wszystkich zebranych wynikow
        for(k=1; k<=n; k++)
            printf("%f \n", xw[k]);
        printf("Czas obliczen: %f sek\n", czas);
    }
    for(i=0; i<=n; i++)
        free(A[i]);

    free(A);
    free(b);
    free(x);
    free(xw);

    MPI_Finalize();
    return 0;
}
