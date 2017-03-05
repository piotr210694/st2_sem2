#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xmintrin.h>
#include <malloc.h>
#include <time.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
int wyraz(float *a, float *b, float *c, int n, float *wyn, int typAdr)
{
	int i;
	__m128, X, Y, Z, ssePi; //deklaracja typu
	ssePi = _mm_set_ps1(3.14f); //ssePi = (3.14,3.14,3.14,3.14)
//wersja pierwsza z adresowaniem argumentów dla SSE za pomocą skalowych zmiennych wskaźnikowych
	if(typAdrp==1)
	{
		__m128 *asse, *bsse, *csse;
		float *wynRef;
		asse = (__m128*)&a[0];
		bsse = (__m128*)&b[0];
		csse = (__m128*)&c[0];
		wynReg = &wyn[0];

		for(i = 0; i < n / 4; i++)
		{
			W = _mm_mul_ps(*asse, *bsse);
			X = _mm_add_ps(W, *csse);
			Y = _mm_add_ps(X, ssePi);
			Z = _mm_sqrt_ps(Y);
			_mm_store_ps(wynRef, Z);
//specyficzne zmienne wskaźnikowe obsługujące typy wektorowe _w tym przypadku 4 liczby float, których inkrementacja oznacza przesunięcie okresu w pamięci o 4 elementy tablicy
			asse++;
			bsse++;
			wynRef += 4;			
		}
	}
	else
	{
		__m128 asse, bsse, csse;
		for(i = 0; i < n; i += 4)
		{
			asse = _mm_load_ps(&a[i]);
			bsse = _mm_load_ps(&b[i]);
			csse = _mm_load_ps(&c[i]);
			W = _mm_mul_ps(asse, bsse);
			X = _mm_add_ps(W, csse);
			Y = _mm_add_ps(Y, ssePi);
			Z = _mm_sqrt_ps(Y);
			_mm_store_ps(&wyn[i], Z);
		}
	}
	//ten tryb adresowanie ładuje 4  32-bitowe wartości typu float

	return 0;
}

