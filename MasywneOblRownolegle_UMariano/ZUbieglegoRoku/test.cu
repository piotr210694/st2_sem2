/* 

ZWYKLE:
gcc test.c -o test
./test

CUDA:
test.cu
nvcc test.cu -o test
./test

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda.h>
#include <cuda_runtime_api.h>

__global__
void funkcja_testowa(){

}

int main(){
	funkcja_testowa<<<1,1>>>();
	printf("CUDA error: %s\n", cudaGetErrorString(cudaGetLastError()));
}
