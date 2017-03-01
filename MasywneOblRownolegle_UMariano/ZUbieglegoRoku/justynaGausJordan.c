//rozwiazywanie rownan liniowych metoda Gaussa-Jordana z wykorzystowaniem z biblioteki MPI
// zainstalowac openmpi?

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
/*
mpd&       
cd Pulpit
mpicc  Z.c -o Z -lm
mpirun -n 1 ./Z

*/

int main(int argc, char *argv[])
{  
double **A,*b,*x,*xw;
double czasKoncowy, czasPoczatkowy,c;
int i,j,k,n;
int ib,ik,ni,kni;
int id,p,korzen=0;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&id);
MPI_Comm_size(MPI_COMM_WORLD,&p);


if(id==korzen){
printf("Podaj wymiar macierzy:\n");
scanf("%d",&n);
}
MPI_Bcast(&n,1,MPI_INT,korzen,MPI_COMM_WORLD);

//inicjalizacja macierzy i wektorow
A=(double**)malloc((n+1)*sizeof(double*));
b=(double*)malloc((n+1)*sizeof(double*));
x=(double*)malloc((n+1)*sizeof(double*));
xw=(double*)malloc((n+1)*sizeof(double*));

for(i=1;i<=n;i++)
{
A[i]=(double*)malloc((n+1)*sizeof(double*));
}

//wypelnianie tab A i b wartosciami
A[1][1]=1;
A[1][2]=2;
A[2][1]=3;
A[2][2]=4;
b[1]=5;
b[2]=6;

//stosujemy czas obliczen
MPI_Barrier(MPI_COMM_WORLD);
czasPoczatkowy=MPI_Wtime();

//dzielimy caly zakres indeksow i w petli  2 po i na podprzedzialach
//ni=(int)(ceil((float)(n)/(float)(p)));
ni=n/p;
ib=id*ni+1;//dolna granica
ik=ib+ni-1;
if(id==p-1)
ik=n;

//wydruk kontrolny
printf("rozmiar ukladu rownan: %d, ilosc procesow: %d, indeksy na proces=%d, dolna granica przedzialu=%d, gorna granica przedzialu=%d \n", n, p,ni,ib,ik);

//alg eliminacji Gausssa-Jordana
for(k=1;k<=n;k++)
{
//petla modyfikacji wierzy ponizej i powyzej k-tego wiersza
    for(i=ib;i<=ik;i++)
{
    if(i==k) continue;//omijamy k wiersz
  
    c=A[i][k]/A[k][k];
    //petla kolumn w danym wierszu
    for(j=k;j<=n;j++)
    A[i][j]=A[i][j]-c*A[k][j];
  
    b[i]=b[i]-c*b[k];
  
}

//rozeslanie wiersza eliminacji
if(k<n){
kni=k/ni;
printf("kni=%d  \n",kni);

MPI_Bcast(A[k+1],n+1,MPI_DOUBLE,kni,MPI_COMM_WORLD);
MPI_Bcast(&b[k+1],1,MPI_DOUBLE,kni,MPI_COMM_WORLD);
}
}

for(k=ib;k<=ik;k++){
x[k]=b[k]/A[k][k];
//wydruk z podprrzedzialow
MPI_Barrier(MPI_COMM_WORLD);
for(k=1;k<=n;k++)
printf("Rozwiazanie: x = %f",x[k]);
//zebranie wszystkich z podprzedzialu
MPI_Gather(&x[ib],ni,MPI_DOUBLE,&xw[ib],ni,MPI_DOUBLE,korzen,MPI_COMM_WORLD);
MPI_Barrier(MPI_COMM_WORLD);
if(id==korzen){
//wydruk wszystkich zebranych wynikow
printf("\n");
for(k=1;k<=n;k++)
printf(" %f\n",xw[k]);
}
czasKoncowy=MPI_Wtime();
printf("Czas obliczen : %lf sek \n",czasKoncowy-czasPoczatkowy);
}
//for(i=0;i<=n;i++){
//free(A[i]);
//}


MPI_Finalize();
return 0;
}
