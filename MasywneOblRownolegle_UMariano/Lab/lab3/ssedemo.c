#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <xmmintrin.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

int wyraz(float *a,float *b,float *c,int n,float *wynik,int TypAdresowania) {
	int i;
	__m128 W,X,Y,Z,ssePi; //Deklaracja typu
	ssePi= _mm_set_ps1(3.14f);
	if (TypAdresowania == 1) 
	{
		//Wersja pierwsza z adresowaniem argumentow 
		//dla SSE za pomoca skalarnych zmiennych wskaźnikowych
		__m128 *asse,*bsse,*csse;
		float *wynref;
		asse=(__m128*)&a[0];
		bsse=(__m128*)&b[0];
		csse=(__m128*)&c[0];
		
		wynref=&wynik[0];
		for (i=0; i<n/4;i++)
		{
			W=_mm_mul_ps(*asse,*bsse);
			X=_mm_add_ps(W,*csse);
			Y=_mm_add_ps(X,ssePi);
			Z=_mm_sqrt_ps(Y);
			_mm_store_ps(wynref,Z); //Specyficzne zmienne obsługujace typy wektorowe,
									//W tym przypadku cztery liczby float
									//KTORYCH INKREMENTACJA OZNACZA, przesuniecie adresu w pamieci o 4 elementy tablicy
			asse++;
			bsse++;
			csse++;
			wynref+=4;
		}
	}
	else
	{
		__m128 asse,bsse,csse;
		for (i=0;i<n;i+=4) 
		{
			asse=_mm_load_ps(&a[i]);
			bsse=_mm_load_ps(&b[i]);
			csse=_mm_load_ps(&c[i]);
			
			W=_mm_mul_ps(asse,bsse);
			X=_mm_add_ps(W,csse);
			Y=_mm_add_ps(X,ssePi);
			Z=_mm_sqrt_ps(Y);
			_mm_store_ps(&wynik[i],Z);			
		}
	}
	
	return 0;
}

int main(int argc,char *argv[]) {
 int n,i;
 float *a,*b,*c,*wynTrad,*wynSSE;
 int typAdr;
 float roznica,normaC,normaE,bladE;
 clock_t start,stop;
 double czas_zwykly,czasSSE;
 int wf;
 n=atoi(argv[1]);
 //rezerwujemy [amiec 
 wynTrad=malloc(n*sizeof(float));
 a=_mm_malloc(n*sizeof(float),16); // Rezerwacja pamieci z pakowanie do 16 bitow
 				   // (specjalne allokacje z wyrowaniem adresu poczatkowego do granic blokow 16 bajtowych)_
  				   // tego wymaga operacja wymiany danych z SSE
 b=_mm_malloc(n*sizeof(float),16);
 c=_mm_malloc(n*sizeof(float),16); 
 wynSSE=_mm_malloc(n*sizeof(float),16);
 
 for(i=0;i<n;i++)
 {
   a[i]=10.33*sin(100.0*(n-i));
   b[i]=2.25*cos(i);
   c[i]=50*max(0.5,sin(7.3*i));
 }
 
 start=clock();
 for(i=0;i<n;i++) wynTrad[i]=sqrt(a[i]*b[i]+c[i]+3.14);
 stop=clock();
 czas_zwykly=((double)(stop-start))/CLOCKS_PER_SEC;
 
 printf("Czas tradycyjny wynosi: %f\n",czas_zwykly);
 
 for (typAdr=1; typAdr<=2;typAdr++) {
	printf("Typ adresowania %d\n",typAdr);
	start=clock();
	wf=wyraz(a,b,c,n,wynSSE,typAdr);
	stop=clock(); 
	czasSSE=((double)(stop-start))/CLOCKS_PER_SEC;
	roznica=wynTrad[0]-wynSSE[0];
	bladE=pow(roznica,2);
	normaC=fabs(roznica);
	
	for(i=1;i<n;i++) {
	   roznica=wynTrad[i]-wynSSE[i];
	   bladE+=pow(roznica,2);
	   normaC=max(normaC,roznica);
	} 
 	normaE=sqrt(bladE);
	 printf("Odleglosc Euklidesa= %f\n Odleglosc Cnebynewa = %f\n",normaE,normaC);
 	printf("Czas zwykly %5.1f (ms) , Czas SSE %5.1f (ms)\n",czas_zwykly*1000,czasSSE*1000);
 }
 free(wynTrad);
 _mm_free(a);
 _mm_free(b);
 _mm_free(c);
 _mm_free(wynSSE);
 
  return 0;
}