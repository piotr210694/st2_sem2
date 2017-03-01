// linux mint
// pisanie programu
   // nano program1.c
// kompilacja
   // gcc -o1 -msse -fopenmp program1.c -o program1 -lm
// uruchomienie
   // ./program1 8000000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xmmintrin.h>
#include <malloc.h>
#include <time.h>
#include <omp.h>
#define max(a,b) ((a) > (b) ? (a) : (b))

int wyraz (float *a, float *b, float *c, int n, float *wyn, int typAdr){

	int i;
	
	__m128 W, X, Y, Z, ssePi;
	
	ssePi = _mm_set_ps1(3.14f); // ssePi = [3.14, 3.14, 3.14, 3.14]
	
	if (typAdr == 1) { // wersja pierwsza z adresowaniem argumentow
					   // dla SSE za pomocą skalarnych zmiennych wskaźnikowych
		__m128 *asse, *bsse, *csse;
		
		float *wynRef;
		asse = (__m128*) &a[0];
		bsse = (__m128*) &b[0];
		csse = (__m128*) &c[0];
		wynRef = &wyn[0];
		for (i=0; i<n/4; i++){
			W = _mm_mul_ps(*asse, *bsse);
			X = _mm_add_ps(W, *csse);
			Y = _mm_add_ps(X, ssePi);
            Z = _mm_sqrt_ps(Y);
			_mm_store_ps(wynRef, Z);
            asse++;
            bsse++;
            csse++;
            wynRef += 4;
		}
	}
	else            // wersja druga z adresowaniem argumentów
                    // za pomocą adresów elementów tablic
    {
        __m128 asse, bsse, csse;

        for(i = 0; i < n; i += 4) {
            asse = _mm_load_ps(&a[i]);
            bsse = _mm_load_ps(&b[i]);
            csse = _mm_load_ps(&c[i]);
            W =  _mm_mul_ps(asse, bsse);
            X =  _mm_add_ps(W, csse);
            Y =  _mm_add_ps(X, ssePi);
            Z = _mm_sqrt_ps(Y);
            _mm_store_ps(&wyn[i], Z);
        }
    }
    return 0;
}
int main(int argc, char* argv[]){
    int n, i;
    float *a, *b, *c, *wynTrad, *wynSSE;
    int typAdr;
    float roznica, normaC, bladE, normaE;
    clock_t start, end;
    double czas_zwykly, czas_SSE;
	double end_t, begin_t;
    int wf;

    n = atoi(argv[1]);
    wynTrad = malloc(n * sizeof(float));
    a = _mm_malloc(n * sizeof(float), 16);
                           // specjalne alokacje z wyrównaniem
    b = _mm_malloc(n * sizeof(float), 16);
                           // adresu początkowego do granic bloków
    c = _mm_malloc(n * sizeof(float), 16);
                           // 16-bajtowych (tego wymagają operacje
    wynSSE = _mm_malloc(n * sizeof(float), 16);
                           // wymiany danych z SSE)

    for (i = 0; i < n; i++){
        b[i] = 10.33 * sin(100.*(n - i));
        a[i] = 2.25 * cos(i);
        c[i] = 50 * max(0.5, sin(7.3 * i));
    }

    begin_t = omp_get_wtime();
    #pragma omp parallel for default(none) private(i) shared(a,b,c,n,wynTrad)
	for(i=0; i<n; i++)
	    wynTrad[i]=sqrt(a[i]*b[i]+c[i]+3.14);

    end_t = omp_get_wtime();

	printf("czas z dwoma watkami: %5.1f\n", (end_t - begin_t)*1000);


    start = clock();
	for(i=0; i<n; i++)
	    wynTrad[i]=sqrt(a[i]*b[i]+c[i]+3.14);
    end = clock();


    czas_zwykly = ((double) (end - start)) / CLOCKS_PER_SEC;
    for(typAdr = 1; typAdr <= 2; typAdr++){
        printf("Typ adresowania: %d\n",typAdr);
        start = clock();
        wf = wyraz(a, b, c, n, wynSSE, typAdr);
        end = clock();
        czas_SSE = ((double) (end - start)) / CLOCKS_PER_SEC;
        roznica = wynTrad[0] - wynSSE[0];
        bladE = pow(roznica, 2);
        normaC = fabs(roznica);
        for(i = 1; i < n; i++){
            roznica = wynTrad[i] - wynSSE[i];
            bladE += pow(roznica, 2);
            normaC = max(normaC, roznica);
        }
        normaE = sqrt(bladE);
        printf(
	  " Odleglosc Euklidesa= %f\n Odleglosc Czebyszewa=%f\n",
                                                  normaE, normaC);
        printf(" Czas zwykly: %5.1f [ms], Czas SSE: %5.1f [ms] \n",
                              czas_zwykly * 1000, czas_SSE * 1000);
    }
    free(wynTrad);
    _mm_free(a); _mm_free(b); _mm_free(c); _mm_free(wynSSE);
    return 0;
}


