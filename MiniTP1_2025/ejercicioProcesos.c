#include <stdio.h>
#include <unistd.h> // Para utilizar sleep()

// Declaración de funciones
int validarEntrada(const char *mensaje, int *valor, int min, int max);
int realizarOperacion(int tipoOperacion, int numero1, int numero2);

int main() {
    // Defino variables
    int tipoOperacion, numero1, numero2;

    // Solicitar operación
    if (!validarEntrada("Elija una operación:\n1. Sumar\n2. Restar\n3. Multiplicar\n4. Dividir\n", &tipoOperacion, 1, 4)) {
        printf("Operación no válida. Finalizando programa.\n");
        return 1;
    }

    // Solicitar números
    if (!validarEntrada("Ingrese el primer número:\n", &numero1, -2147483648, 2147483647) ||
        !validarEntrada("Ingrese el segundo número:\n", &numero2, -2147483648, 2147483647)) {
        printf("Número no válido. Finalizando programa.\n");
        return 1;
    }

    // Simular actividad para observar el estado "R" (Running), haciendo que consuma CPU.
    printf("Realizando cálculos intensivos...\n");
    for (long i = 0; i < 10000000000; i++);

    // Realizar operación
    if (!realizarOperacion(tipoOperacion, numero1, numero2)) {
        printf("Error al realizar la operación. Finalizando programa.\n");
        return 1;
    }

    // Finalizar
    printf("Finalizando el programa...\n");
    sleep(5);
    return 0;
}

int validarEntrada(const char *mensaje, int *valor, int min, int max) {
    printf("%s", mensaje);
    if (scanf("%d", valor) != 1 || *valor < min || *valor > max) {
        return 0; // No valido
    }
    return 1; // Valido
}

int realizarOperacion(int tipoOperacion, int numero1, int numero2) {
    int resultado;
    switch (tipoOperacion) {
        case 1: resultado = numero1 + numero2; printf("Resultado de la suma: %d\n", resultado); break;
        case 2: resultado = numero1 - numero2; printf("Resultado de la resta: %d\n", resultado); break;
        case 3: resultado = numero1 * numero2; printf("Resultado de la multiplicación: %d\n", resultado); break;
        case 4:
            if (numero2 == 0) {
                printf("Error: División por cero no permitida.\n");
                return 0;
            }
            resultado = numero1 / numero2;
            printf("Resultado de la división: %d\n", resultado);
            break;
        default: return 0; // Operación no válida
    }
    return 1; // Operación realizada con éxito
}