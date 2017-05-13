
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define N 2000000 //definiowanie przedzialu 2..N
#define S (int)sqrt(N)
#define M N/10


int main(int argc, char *argv[]){
	long int a[S+1]; //tablica pomocnicza
	long int podzielniki[S+1]; //podzielniki z przedzialu 2//sqrt(N)
	long int pierwsze[M]; //liczby pierwsze w podprzedzialach
	long int wszystkiepierwsze[M]; //wszystkie liczby pierwsze
	long int liczba, reszta =0;
	int dl_podpn; // dlugosc podprzedzialu liczb
	int llpier; //liczba liczb pierwszych
	int lpodz; //liczba podprzedzialow w tablicy przedzialy
	int liczba_p; //sumaryczna liczba znalezionych liczb pierwszych
	int id,p; //id procesu, liczba procesów
	
	int *liczby_grom; //tablica ilosci gromadzonych liczb pierwszych
	int *przem; //tablica przemieszczen (przesuniec)
	int i,k;
	double czas_1,czas_2; //do pomiaru czasu
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	MPI_Barrier(MPI_COMM_WORLD);
	if (id == 0 ) czas_1 = MPI_Wtime();
	MPI_Comm_size(MPI_COMM_WORLD,&p); 
	
//	if (id == 0 ) czas_1 = MPI_Wtime();
	
	//Etap A
	//wyznaczanie podzielnikow w przedziale 2..S
	for (i=2;i<=S;i++) a[i]=1; //inicjowanie tablicy
	i=0;
	llpier=0;
	lpodz=0;
	
	while(i<=S)
	{
	  i++;
	  	if (a[i]==1)
	 	{
	  		pierwsze[llpier++]=podzielniki[lpodz++]=i; // Zapamietanie liczby pierwsszej
	  		//wykreslanie liczb zlozonych bedacych wielokrotnosciami i
	  		for (k=i*i; k<=S;k+=i) a[k]=0;			
	 	}	
	}
	
	//Etap B
	//rownolegle wyznaczanie liczb pierwszych w podprzedzialach
	//2.............S.S+1..................N
	//Czyli przedzial od S+1 do N dzielimy na p podprzedzialow (p ilosc procesow)
	dl_podpn=(N-S)/p; //Obliczenie dlugosci podprzedzialow
	if ((N-S) %p != 0) dl_podpn++;
	
	if (id > 0) llpier=0;
	for(liczba=S+1+dl_podpn*id; liczba< S+1+dl_podpn*(id+1); liczba++)
	{
		if (liczba <= N) 
		{
		  for(k=0; k<lpodz;k++) 
		  {
			  reszta=(liczba%podzielniki[k]);
			  if (reszta == 0 ) break; // liczba zlozona
		  }
		  if (reszta !=0) pierwsze[llpier++]=liczba; //znajdujemy liczbe pierwsza
		}
	}
	
	liczby_grom=(int*) malloc(p*sizeof(int));
	MPI_Gather(&llpier,1,MPI_INT,liczby_grom,1,MPI_INT,0,MPI_COMM_WORLD);
	przem=(int*) malloc(p*sizeof(int));
	przem[0]=0;
	liczba_p=liczby_grom[0]; //sumaryczna ilosc wyznaczonych liczb pierwszych
	for(i=1;i<p;i++) {
	 przem[i]=przem[i-1]+liczby_grom[i-1];
	 liczba_p+=liczby_grom[i];
	}	 
	
	MPI_Barrier(MPI_COMM_WORLD);
	if (id == 0) {
		printf("Liczba procesow %d, N: %d, dlugosc podprzedzialow %d\n",p,N,dl_podpn);
		printf("Sumarczyna liczba liczb pierwszych %d\n",p);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printf("Liczba liczb pierwszych w przedziale %d, %d\n",id,llpier);
	MPI_Gatherv(pierwsze,llpier,MPI_LONG,wszystkiepierwsze,liczby_grom,przem,MPI_LONG,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if (id == 0) {
	   czas_2= MPI_Wtime();
	   printf("Czas: %f sekund\n",(czas_2-czas_1));
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if (id == 0) {
		printf("Liczby pierwsze\n");
	/*	for(k=0;k<liczba_p;k++) {
			printf(" %ld ",wszystkiepierwsze[k]);
		}
	*/	
		printf("\nNajwieksza liczba pierwsza: %ld\n",wszystkiepierwsze[liczba_p-1]);
	}	
			
	MPI_Finalize();
return 0;
}
