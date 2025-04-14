#include <stdio.h>    // Para printf
#include <pthread.h>  // Para trabajar con hilos

// Función que realiza el cálculo aritmético
void* calculo_aritmetico(void* arg) {
    int contador = 0;

    while (contador < 2147483647) {
        contador = contador + 1;
    }

    printf("Hilo %ld finalizó su cálculo.\n", (long)arg);
    return NULL;
}

int main() {
    pthread_t hilos[5]; // Arreglo para almacenar los identificadores de los hilos

    // Crear 5 hilos, cada uno ejecutando la función calculo_aritmetico
    for (long i = 0; i < 5; i++) {
        if (pthread_create(&hilos[i], NULL, calculo_aritmetico, (void*)i) != 0) {
            perror("Error al crear el hilo");
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < 5; i++) {
        if (pthread_join(hilos[i], NULL) != 0) {
            perror("Error al unir el hilo");
            return 1;
        }
    }

    printf("Todos los hilos han finalizado.\n");
    return 0;
}
