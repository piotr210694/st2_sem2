#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <omp.h>
#define N 10000
//sortowanie tablicy
int main(int argc, char *argv[]) 
{
float a[N]; //tablica przed sortowaniem
float b[N]; //tablica po sortowaniu
int ind[N]; //index tablicy
int i,j;
double begin_t, end_t;

begin_t = omp_get_wtime();

for(i=0;i<N;i++)
{
a[i]=random()/1000000.0;
}

#pragma omp parallel for shared(a,b,ind) private(i,j)
for(i=0;i<N;i++) 
{
	ind[i]=0;
	for(j=0;j<N;j++) //wyznaczanie indexu dla elementu a[i]
	{
		if( (a[i]>a[j]) || ((a[i]==a[j]) && (i>j)) )
		{
			ind[i]++;
		}
		
	}
	b[ind[i]]=a[i]; //krok sortowania
}
/*
printf("\na: \n"); //przed sortowaniem
for(i=0;i<N;i++)
{
	printf("%6.1f ", a[i]);
}

printf("\nb: \n"); //po sortowaniu
for(i=0;i<N;i++)
{
	printf("%6.1f ", b[i]);
}
printf("\n");
*/
end_t = omp_get_wtime();
printf("Czas obliczen %lf \n", end_t-begin_t);
return 0;
}
