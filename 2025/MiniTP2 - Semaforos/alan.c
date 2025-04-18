#include <stdio.h>
#include <semaphore.h>  // semáforos
#include <pthread.h>    // hilos
#include <unistd.h>     // sleep
#include <stdlib.h>     // rand y srand
#include <time.h>       // Para inicializar la semilla de rand

// Declaración semáforos
sem_t semA, semB;

// Declaración de variables globales
int count_B = 0; 
int dino = 5;

// Función para imprimir el estado del uso de la fotocopiadora
void imprimir_estado(int dino_numero, char tipo, int enfriamiento) {
    printf("El dino %d uso la fotocopiadora, el tipo fue %c y %s.\n", 
           dino_numero, 
           tipo, 
           enfriamiento ? "hubo enfriamiento" : "no hubo enfriamiento");
}

void esperar_enfriar() {
    printf("Esperando a que la fotocopiadora B se enfríe...\n");
    sleep(5); // Simula el tiempo de enfriamiento
    printf("Fotocopiadora B se ha enfriado.\n");
}

void* usar_fotocopiadora(void* arg) {
    char tipo = *(char*)arg; // Tipo de fotocopiadora (A o B)

    for (int i = 0; i < 5; i++) {
        if (tipo == 'A') {
            sem_wait(&semA); // Espera al semáforo A
            sleep(3); // Simula el uso de la fotocopiadora A
            imprimir_estado(i + 1, 'A', 0); // No hay enfriamiento para A
            sem_post(&semB); // Libera el semáforo B
        } else if (tipo == 'B') {
            sem_wait(&semB); // Espera al semáforo B
            count_B++;
            sleep(3); // Simula el uso de la fotocopiadora B

            if (count_B == 2) {
                imprimir_estado(i + 1, 'B', 1); // Hubo enfriamiento
                esperar_enfriar(); // Enfría la fotocopiadora después de 2 usos
            } else {
                imprimir_estado(i + 1, 'B', 0); // No hubo enfriamiento
            }

            if (count_B > 2) {
                sem_post(&semA); // Libera el semáforo A para cambiar a Tipo A
                count_B = 0; // Reinicia el contador de B después de imprimir
            } else {
                sem_post(&semB); // Libera el semáforo B para continuar con Tipo B
            }
        }
    }

    pthread_exit(NULL);
}

void colocar_trabajo_en_bandeja(int random_choice) {
    sleep(2); // Simula el tiempo de colocación del trabajo

    // Decidir qué tipo de fotocopiadora usar basado en el valor aleatorio
    if (random_choice == 0) {
        sem_post(&semA); // Libera el semáforo A para usar la fotocopiadora A
    } else {
        sem_post(&semB); // Libera el semáforo B para usar la fotocopiadora B
    }
}

int main() {
    pthread_t tpid_A, tpid_B;
    char tipoA = 'A'; // Tipo de fotocopiadora A
    char tipoB = 'B'; // Tipo de fotocopiadora B

    // Inicialización de los semáforos
    sem_init(&semA, 0, 1); // Inicializa semáforo A en 1 (activo)
    sem_init(&semB, 0, 0); // Inicializa semáforo B en 0 (bloqueado)

    // Inicialización de la semilla para números aleatorios
    srand(time(NULL));

    // Creación de los hilos
    pthread_create(&tpid_A, NULL, usar_fotocopiadora, &tipoA);
    pthread_create(&tpid_B, NULL, usar_fotocopiadora, &tipoB);

    // Simula la colocación de trabajos en la bandeja
    for (int i = 0; i < dino; i++) {
        int random_choice = rand() % 2; // Genera un número aleatorio entre 0 y 1
        colocar_trabajo_en_bandeja(random_choice);
    }

    // Espera a que los hilos terminen
    pthread_join(tpid_A, NULL);
    pthread_join(tpid_B, NULL);

    // Destruye los semáforos
    sem_destroy(&semA);
    sem_destroy(&semB);

    printf("\n"); // Salto de línea final
    return 0;
}