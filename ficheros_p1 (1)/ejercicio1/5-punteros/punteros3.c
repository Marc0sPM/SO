#include <stdio.h>
#include <stdlib.h>

int nelem;
int c;

int main(void)
{
	int *ptr;
	int i;

	c = 37;
	nelem = 127;
	ptr = (int*) malloc(nelem * sizeof(int));
	for (i=0; i < nelem; i++)
		ptr[i] = i;

	printf("ptr=%p ptr[0]= %d ptr[13]=%d \n", ptr ,ptr[0], ptr[13]);

	//Puntero auxiliar para no perder referencia a la memoria reservada con malloc()
	int *ptr2;
	ptr2 = ptr;
	
	ptr = &c;
	printf("ptr=%p ptr[0]= %d ptr[13]=%d \n", ptr, ptr[0], ptr[13]);

	free(ptr2);

}

