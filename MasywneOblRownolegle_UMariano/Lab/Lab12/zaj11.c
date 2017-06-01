#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define N 4000000
#define S (int)sqrt(N)
#define M N/10
int main(int argc, char** argv)
{
  long int a[S+1];
  long int podzielniki[S+1];
  long int pierwsze[M];
  long int wszystkiepierwsze[M];
  long int liczba; 
  long int reszta = 0;
  int dl_podprz;
  int llpier;
  int lpodz;
  int liczba_p;
  int id, p;
  int *liczby_grom;
  int *przem;
  int i,k;
  double czas1; 
  double czas2;
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Barrier(MPI_COMM_WORLD);
  if(id==0) czas1=MPI_Wtime();
  MPI_Comm_size(MPI_COMM_WORLD, &p);  
  for(i=2;i<=S;i++) a[i]=1;
  i=0; llpier=0; lpodz=0;
  while(i<=S)
  {
    i++;
    if(a[i]==1)
    {
      pierwsze[llpier++]=podzielniki[lpodz++]=i;
      for(k=i+i; k<=S; k+=i) a[k]=0;
    }
  }
  dl_podprz=(N-S)/p;
  if((N-S)%p!=0) dl_podprz++;
  if(id>0) llpier=0;
  for(liczba=S+1+dl_podprz*id; liczba<S+1+dl_podprz*(id+1);liczba++)
  {
    if(liczba<=N)
    {
      for(k=0; k<lpodz; k++)
      {
        reszta=(liczba%podzielniki[k]);
        if(reszta==0) break;
      }
    if(reszta!=0) pierwsze[llpier++]=liczba;
    }
  }
  liczby_grom=(int*)malloc(p*sizeof(int));
  MPI_Gather(&llpier, 1, MPI_INT, liczby_grom, 1, MPI_INT, 0, MPI_COMM_WORLD);
  przem=(int*) malloc(p*sizeof(int));
  przem[0]=0;
  liczba_p=liczby_grom[0];
  for(i=1;i<p;i++)
  {
    przem[i]=przem[i-1]+liczby_grom[i-1];
    liczba_p+=liczby_grom[i];
}
  MPI_Barrier(MPI_COMM_WORLD);
  if(id==0)
  {
    printf("Liczba procesow: %d, N: %d, dlugosc podprzedzialow: %d \n", p, N, dl_podprz);
    printf("Sumaryczna liczba liczb pierszych: %d \n", liczba_p);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  printf("liczba liczb pierwszych w przedziale %d: %d \n", id, llpier);
  MPI_Gatherv(pierwsze,llpier,MPI_LONG,wszystkiepierwsze, liczby_grom,przem, MPI_LONG,0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if(id==0)
  {
    czas2=MPI_Wtime();
    printf("Czas: %f sec \n", czas2-czas1);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if(id==0)
  {
    for(k=0;k<liczba_p;k++)
    {
     // printf(" %d \n",wszystkiepierwsze[k]);
    }
  printf("\n Najwieksza liczba pierwsza: %ld \n", wszystkiepierwsze[liczba_p-1]); 
  }
  MPI_Finalize();
  return 0;
}
