#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int addRepeats(int v, int r);
int value;
int repeats;

void main () {
	printf("Esta funcion permite sumar un valor segun cantidad de repeticiones indicada \n\n");

	printf("Indicar valor a sumar \n");
	scanf("%d", &value);

	printf("Indicar numero de repeticiones \n");
	scanf("%d", &repeats);

	printf ("El resultado de sumar %d, el numero %d es %d \n", repeats, value, addRepeats(value, repeats)  );
}

int addRepeats(int v, int r) {
	int sum=0;
	int g;

	for (g=0; g < r; g++){
		sum += v;
	}

	return sum;
}