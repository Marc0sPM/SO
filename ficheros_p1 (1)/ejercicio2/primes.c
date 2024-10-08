/**
 *  This program calculates the sum of the first n prime
 *  numbers. Optionally, it allows the user to provide as argument the
 *  value of n, which is 10 by default.
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * This function takes an array of integers and returns the sum of its n elements.
 * @param arr array de numeros que se deben sumar
 * @param n numero de elementos del array
 * @return la suma total de todos los elementos contenidos en el array
 */
int sum(int *arr, int n);

/**
 * This function fills an array with the first n prime numbers.
 * @param result array de numeros en los que se van a almacenar los numeros primos
 * @param n cantidad de numeros primos que se solicitan
 */
void compute_primes(int* result, int n);

/**
 * This function returns 1 if the integer provided is a prime, 0 otherwise.
 */
int is_prime(int x);

int main(int argc, char **argv) {
  int n = 10; // by default the first 10 primes
  if(argc == 2) {
    n = atoi(argv[1]);
  }
  int* primes = (int*) malloc(n*sizeof(int));
  compute_primes(primes, n);

  int s = sum(primes, n);
  printf("The sum of the first %d primes is %d\n", n, s);

  free(primes);
  return 0;
}

int sum(int *arr, int n) {
  int i;
  int total = 0;
  for(i=0; i<n; i++) {
    total += arr[i];
  }
  return total;
}

void compute_primes(int* result, int n) {
  int i = 0;
  int x = 2; // Numero que se comprueba si es primo
  while(i < n) {
    if(is_prime(x)) {
      result[i] = x;
      i++;
    }
    // la x se aumende de a uno despues de comprobar si es primo
    x++;
  }
  return;
}

int is_prime(int x) {
  if(x < 0 ) return 0;
  if(x == 2) return 1;
  if(x % 2 == 0) return 0;
  for(int i=3; i<x; i+=2) {
    if(x % i == 0) {
      return 0;
    }
  }
  return 1;
}
