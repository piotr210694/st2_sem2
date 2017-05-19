#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 25
#define S (int)sqrt(N)
#define M N / 10

int main(int argc, char * argv[])
{
    long int a[S + 1];
    long int pierwsze[M];
    long int i, k, liczba, reszta, suma, lpmax;
    long int lpodz = 0;
    long int llpier = 0;

    for (i = 2; i <= S; i++)
        a[i] = 1;
    
    for (i = 2; i <= S; i++)
    {
        if (a[i] == 1)
        {
            pierwsze[llpier++] = i;
            for (k = i * i; k <= S; k += i)
                a[k] = 0;
        }
    }

    lpodz = llpier;
    printf("Liczba liczb pierwszych: %ld\n", llpier);
    printf("Liczba podzielnikow: %ld\n", lpodz);
    printf("Najwieksza liczba pierwsza: %ld\n", pierwsze[llpier - 1]);
    printf("Wszystkie podzielniki ****\n");
    for (i = 0; i < llpier; i++)
    {
        printf(" %ld ", pierwsze[i]);
        if (i % 10 == 0 && i != 0)
            printf("\n");
    }
    printf("\n");

    for (liczba = S + 1; liczba <= N; liczba++)
    {
        for (k = 0; k < lpodz; k++)
        {
            reszta = liczba % pierwsze[k];

            if (reszta == 0)
                break;
        }

        if (reszta != 0)
        {
            pierwsze[llpier++] = liczba;   
        }
    }

    printf("Liczba liczb pierwszych: %ld\n", llpier);
    suma = 0;
    lpmax = 0;
    for (i = 0; i < llpier; i++)
    {
        suma += pierwsze[i];
        if (pierwsze[i] > lpmax)
            lpmax = pierwsze[i];
    }

    printf("Najwieksza l. pierwsza: %ld %ld\n", lpmax, pierwsze[llpier - 1]);
    printf("Suma liczb pierwszych: %ld\n", suma);

    return 0;
}