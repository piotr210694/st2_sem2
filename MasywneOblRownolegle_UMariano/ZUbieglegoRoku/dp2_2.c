/*
algorytm równoległy wyznaczania liczb pierwszych
sito Eratostenesa
2 etapy
Dzielimy przedział [2,n]
A: [2, sqrt(n)] ; B:[sqrt(n)+1,n]
w A - sito Eratostenesa
wyznacza się liczby pierwsze - podzielniki
A = [2,25]

*/

// gcc -o1 -msse -fopenmp dp2_2.c -o dp2_2 -lm
// env OMP_NUM_THREADS=1 ./dp2_2

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <omp.h>

#define N 10000000
#define S (int)sqrt(N)
#define M N/10

int main(int argc, char *argv[]) 
{
long int a[S+1]; //tablica pomocnicza
long int podzielniki[S+1]; //podzielniki z zakresu od 2...S
long int pierwsze[M]; //liczby pierwsze w przedziale 2...N
long int i,k,liczba,reszt,reszta,suma;
long int lpodz=0; //liczba podzielnikow w tablicy podzielniki
long int llpier=0; //liczba liczb pierwszych w tablicy pierwsze
double begin_t, end_t;

printf("--------------------------------------------------\n");
/*wyznaczanie podzielnikow z przedzialu 2...S*/

//#pragma omp parallel for shared (a)
for(i=0;i<=S;i++) //inicjalizacja
	a[i]=1;


//#pragma omp parallel for private(i) shared (a,pierwsze,podzielniki,llpier,lpodz)
for(i=2;i<=S;i++) //inicjalizacja
//#pragma omp critical
{

	
	if(a[i]==1)
	{
		pierwsze[llpier++]=podzielniki[lpodz++]=i; // zapamietanie liczby pierwszej
		//wykreslenie liczb zlozlonych bedacych wielokrotnosciami i
		for(k=i+i;k<=S;k+=i)
		{
			a[k]=0;
		}
	}

}
printf("Liczba liczb pierwszych %ld \n", llpier);
printf("Liczba podzielnikow %ld \n", lpodz);
printf("Najwieksza liczba pierwsza %ld \n", pierwsze[llpier-1]);

/*
printf("Wszystkie podzielniki: %ld \n", llpier);
for(i=0;i<llpier;i++)
{
printf(" %ld ",podzielniki[i]);
}
printf("\n");
*/

// CZĘŚĆ II // równoległe obliczanie liczb pierwszych
printf("--------------------------------------------------\n");


begin_t = omp_get_wtime();
#pragma omp parallel for private(k,liczba,reszta) shared (llpier, lpodz, pierwsze, podzielniki)
for ( liczba = S+1 ; liczba <= N ; liczba ++ ){
    for ( k=0; k < lpodz ; k++ ) {
	reszta = ( liczba % podzielniki [k] ) ;
	if (reszta == 0 ) break; // liczba złożona
    }
    if (reszta != 0){
	#pragma omp critical
        pierwsze[llpier++]=liczba; // zapamietanie liczby pier? 
        }
}
end_t = omp_get_wtime();
printf("Liczba liczb pierwszych: %ld \n", llpier);
suma = 0;
for (i=0; i< llpier; i++){
	suma = suma + pierwsze[i];
	}

printf("Suma liczb pierwszych: %ld \n",suma);

// wypisanie
/*
printf("Wszystkie liczby pierwsze \n");
for (i=0; i<llpier; i++) {
	printf(" %ld ", pierwsze[i]);
	if (i%10 == 0) printf("\n");
	}
*/

printf("\nNajwieksza liczba pierwsza: %ld \n", pierwsze[llpier-1]);
printf("Czas obliczeń liczb pierwszych %lf sek \n", end_t-begin_t);

	
///////////////////////

return 0;
}
