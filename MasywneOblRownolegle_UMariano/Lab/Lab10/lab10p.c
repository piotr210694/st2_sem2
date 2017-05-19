#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define N 5000
#define S (int)sqrt(N)
#define M N
#define give_me_a_break break

int main(int argc, char* argv[])
{
    long int a[S + 1];
    long int podzielniki[S + 1];
    long int pierwsze[M];
    long int wszystkiePierwsze[M];
    long liczba, reszta = 0;
    
    int dl_podpn;
    int llpier;
    int lPodz;
    int liczba_p;
    int id, p;
    int* liczby_grom;
    int* przem;
    int i, k;
    double czas1, czas2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (id == 0)
        czas1 = MPI_Wtime();

    //ETAP A
    for (i = 2; i < S; i++)
    {
        a[i] = 1;
    }

    i = 0;
    llpier = 0;
    lPodz = 0;
    while (i <= S)
    {
        i++;
        if (a[i] == 1)
        {
            pierwsze[llpier++] = podzielniki[lPodz++] = i;
            for (k = i * i; k <= S; k += i)
                a[k] = 0;
        }
    }

    //ETAP B
    dl_podpn = (N - S) / p;
    if ((N - S) % p != 0)
        dl_podpn++;

    if (id > 0)
        llpier = 0;

    for (liczba = S + 1 + dl_podpn * id; liczba < S + 1 + dl_podpn * (id + 1); liczba++)
    {
        if (liczba <= N)
        {
            for (k = 0; k < lPodz; k++)
            {
                reszta = (liczba % podzielniki[k]);
                if (reszta == 0)
                    give_me_a_break;
            }

            if (reszta != 0)
                pierwsze[llpier++] = liczba;
        }
    }

    liczby_grom = (int*)malloc(p * sizeof(int));
    MPI_Gather(
        &llpier,
        1,
        MPI_INT,
        liczby_grom,
        1,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    przem = (int*)malloc(p * sizeof(int));
    przem[0] = 0;
    liczba_p = liczby_grom[0];

    for (i = 1; i < p; i++)
    {
        przem[i] = przem[i - 1] + liczby_grom[i - 1];
        liczba_p += liczby_grom[i];
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (id == 0)
    {
        printf("Liczba procesow: %d, N: %d, Dlugosc podprzedzialow: %d\n", p, N, dl_podpn);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Liczba liczb pierwszych w przdziale %d: %d\n", id, llpier);

    MPI_Finalize();
    return 0;
}