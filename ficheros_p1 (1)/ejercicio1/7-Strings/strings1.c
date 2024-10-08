#include <stdio.h>
#include <string.h>

int main()
{
	char msg[10]; /* array of 10 chars */
	char *p;      /* pointer to a char */
	char *original = "Bonjour"; /* pointer to chain Bonjour*/
	char msg2[28] = "Hello";  /* msg2 = 'H' 'e' 'l' 'l' 'o' '\0' */

	p = "Bonjour"; // tener cuidado de hacer esto. Cadena literal es const. Al modificar puede dar error

	//Uso de variable no inicializada msg -> ERROR EN EJECUCION
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);
	printf("dir de 'B': %p  dir de 'j': %p\n", &p[0], &p[3]);

	p = msg2; 
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p[0] = 'H', p[1] = 'i', p[2] = '\0'; // Como se modifica lo que esta
	// en la direccion de memoria a la que apunta p, se modifica tambien msg2
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("msg len: %lu p len %lu msg2 len %lu\n", strlen(msg), strlen(p), strlen(msg2));
	printf("msg size: %lu p size %lu msg2 size %lu\n", sizeof(msg), sizeof(p), sizeof(msg2));

	msg[0] = 'B', msg[1] = 'y';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
}

