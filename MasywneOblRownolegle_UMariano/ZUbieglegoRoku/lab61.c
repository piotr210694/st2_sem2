#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <xmmintrin.h>
#include <omp.h>

int main(int argc, char **argv)
{
	double **A, *b, *x;
	double c;
	clock_t begin_t, end_t;
	double begin_to, end_to;
	long i, j, k, l, n;
	n = atoi(argv[1]);
	//inicjujemy macierze i wektory
	A = (double**)malloc((n + 1) * sizeof(double*));
	b = (double*)malloc((n + 1) * sizeof(double*));
	x = (double*)malloc((n + 1) * sizeof(double*));
	for(i = 1; i <= n; i++)
		A[i] = (double*)malloc((n + 1) * sizeof(double*));
	A[1][1] = 1; A[1][2] = 2;
	A[2][1] = 3; A[2][2] = 4;
	b[1] = 5; b[2] = 6;
	//etap eliminacji zmiennych
//*************************************************************************************************
	begin_to = omp_get_wtime();
	//petla wierszy eliminujacych
	for(k = 1; k <= n; k++){
	#pragma omp parallel for shared(A, b, k, n) private(c, i, j)
		//petla modyfikuj wierszy ponizej k tego wiersza?
		for(i = 1; i <= n; i++){
			if(i == k)
			continue;
			c = A[i][k] / A[k][k];
		//petla kolum w dany wierszu?
			for(j = k; j <= n; j++){
				A[i][j] = A[i][j] - c * A[k][j];
			 }
			 b[i] = b[i] - c * b[k];
		}
	}
	for(l = 1; l <= n; l++)
	x[l] = b[l] / A[l][l];
	/*
	//2, postepowanie odwrotne
	x[n] = b[n] / A[n][n];
	//petla wierszy od modyfikacji?
	for(k = n - 1; k >= 1; k--){
		x[k] = b[k];
		//petla po obliczonych zmienionych
		for(j = k + 1; j <= n; j++){
			x[k] = x[k] - A[k][j] * x[j];
		}
		x[k] = x[k] / A[k][k];
	}*/
	end_to = omp_get_wtime();
	printf("Rozw: x[1], x[2] = %f, %f \n", x[1], x[2]);
	printf("Czas obliczeń: %f s\n", end_to - begin_to);
		
	
return 0;

}
/*
gcc -o lab61 -fopenmp lab61.c
env OMP_NUM_THREADS=2 ./lab61 1000
*/

