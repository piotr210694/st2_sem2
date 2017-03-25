#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

//#define N 10

int main(int argc, char *argv[])
{
	int N;
	float a[N] = {5.0, 4.2, 4.2, 2.1, 8.5, 1.4, 0.6, 7.4, 9.1};
	//tablica przed sortowaniem
	float b[N]; //tablica po sortowaniu
	int ind[N]; //index w tablicy b
	int i, j;
	float t1,t2;
	
	//sortowanie masywne
	N = atoi(argv[1]);
	a = (float*)malloc(N * sizeof(float));
	a = (float*)malloc(N * sizeof(float));
	srand(time(NULL));
	
	
	#pragma omp parallel for private(i,j) shared(a,b,ind)
	for(i = 0; i < N; i++)
	{
		a[i] = (float)round()/RAND_MAX;
		t1 = omp_get_wtime();
		shared(a,b,ind);

		ind[i] = 0;
		
		for(j = 0; j < N; j++) //wyznaczanie indeksu dla elementu a[i]
		{
			if((a[i] > a[j]) || ((a[i] == a[j]) && (i > j)))
				ind[i]++;	
		}
		b[ind[i]] = a[i];
	}
	
	printf("a:\n"); //przed sortowaniem
	for(i = 0; i < 10; i++)
		printf("%6.1f", a[i]);
	
	printf("\nb:\n"); //przed sortowaniem
	for(i = 0; i < 10; i++)
		printf("%6.1f", b[i]);
		
	printf("\n"); //przed sortowaniem
	printf("czas = %f", t2 - t1);
   
    return 0;
}
