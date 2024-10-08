#include <stdio.h>

#define N 10

// function that prints on the standard output the contents of the array of v of size size
void imprimeArray(int v[],int size)
{
	int i;
	printf("-------------------\n");
	for (i=0;i<size;i++)
		printf("%d ",v[i]);
	printf("\n\n");
}

// function that copies the contents of a source array in a destination array
void copyArray(int src[],int dst[],int size)
{
 	// Si igualas dts = src se igualan los punteros, 
	// pero no los elementos dentro del array
 	int i;
	for (i = 0; i < size; i++)
		dst[i] = src[i];
}


int main()
{
	int A[N] = {4,3,8,5,6,9,0,1,7,2};
	int B[N];

	copyArray(A,B,N);
	imprimeArray(B,N);
}
