#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <xmmintrin.h>
#include <omp.h>

//algorytm eliminacji gaussa-jordana

int main(int argc, char *argv[])
{

double **A, *b, *x;
double c;
clock_t begin_t, end_t;
double begin_to, end_to;
long i,j,k,l,n;

n=atoi(argv[1]);

//inicjacja macierzy i wektorów

A=(double**)malloc((n+1)*sizeof(double*));
b=(double*)malloc((n+1)*sizeof(double*));
x=(double*)malloc((n+1)*sizeof(double*));

for(i=1;i<=n;i++)
A[i]=(double*)malloc((n+1)*sizeof(double*));

for(i=1;i<=n;i++)
{
for(j=1;j<=n;j++)
A[j][i]=(double)(i+j)/1000.0f;
b[i]=(double)(i)/1000.f;
}

/*
A[1][1]=1;
A[1][2]=2;
A[2][1]=3;
A[2][2]=4;
b[1]=5;
b[2]=6;
*/
//etap eliminacji zmiennych

begin_t=clock();
begin_to=omp_get_wtime();

for(k=1;k<=n;k++)
{
#pragma omp parallel for shared(A,b,k,n) private(i,j,c)
for(i=1;i<=n;i++)
{
if(i==k){
continue;}
c=A[i][k]/A[k][k];
for(j=k;j<=n;j++)
{
A[i][j]=A[i][j]-c*A[k][j];
}
b[i]=b[i]-c*b[k];
}
}
for(l=1;l<=n;l++)
x[l]=b[l]/A[l][l];

end_t=clock();
end_to=omp_get_wtime();





//printf("ROzwiazanie: x[1], x[2]= %f %f\n",x[1],x[2]);

printf("Czas obliczen OMP (gaussa-jordana): %f.\n",end_to-begin_to);
printf("Czas obliczen zwykly (gaussa-jordana): %f.\n",(double)(end_t-begin_t)/CLOCKS_PER_SEC);


//zwalnianie pamieci

for(i=1;i<n;i++)
free(A[i]);

free(A);
free(b);
free(x);
}