#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Semáforos
sem_t fotocopiadora_sem; // Controla el acceso exclusivo a la fotocopiadora
sem_t bandeja_sem; // Controla el acceso exclusivo a la bandeja
sem_t enfriamiento_sem; // Controla el enfriamiento
sem_t prioridad_sem; // Controla la prioridad A sobre B

// Variables Globales
int trabajos_b_seguidos = 0;
int trabajos_b_en_espera = 0; // 3 en espera
int total_trabajos = 0;

// Primitivas
void usar_fotocopiadora(char tipo_trabajo, int id_dinosaurio) {
    sem_wait(&fotocopiadora_sem);
    printf("Dinosaurio %d usando la fotocopiadora para trabajo tipo %c\n", id_dinosaurio, tipo_trabajo);
    if (tipo_trabajo == 'B') {
        trabajos_b_seguidos++;
        total_trabajos++;
        if (trabajos_b_seguidos == 2) {
            sem_post(&prioridad_sem); // Señala que se necesita un trabajo A
        }
    } else {
        trabajos_b_seguidos = 0; // Reinicia el contador si es trabajo A
        // Debo reiniciar el contador de trabajos B en cola??
    }
    // Simula el tiempo de fotocopiado
    sleep(tipo_trabajo == 'A' ? 1 : 3);
    printf("Dinosaurio %d terminó de usar la fotocopiadora\n", id_dinosaurio);
    sem_post(&fotocopiadora_sem);

    // Si el trabajo era de tipo B, se incrementa el contador de trabajos B en cola"
}

void esperar_enfriamiento() {
    printf("La fotocopiadora necesita enfriarse...\n");
    sleep(5);
    printf("La fotocopiadora está lista para usar\n");
    sem_post(&enfriamiento_sem);
}

void colocar_trabajo_en_bandeja(int id_dinosaurio) {
    sem_wait(&bandeja_sem);
    printf("Dinosaurio %d colocó su trabajo en la bandeja\n", id_dinosaurio);
    // Sumar trabajo de tipo B, en el contador "trabajos_b_en_cola"
    sleep(1); // Simula colocar el trabajo
    sem_post(&bandeja_sem);
}

void imprimir_estado(int id_dinosaurio, char tipo_trabajo, int enfriando) {
    printf("--- Estado de la Fotocopiadora ---\n");
    printf("Dinosaurio usando: %d\n", id_dinosaurio);
    printf("Tipo de trabajo: %c\n", tipo_trabajo);
    printf("Enfriando: %s\n", enfriando ? "Sí" : "No");
    printf("-------------------------------\n");
}

// Funcion thread del dinosaurio
void *dinosaurio(void *arg) { // 
    int id_dinosaurio = *(int *)arg;
    char tipo_trabajo;
    // Simula la decisión del tipo de trabajo
    if (rand() % 100 < 50) {
        tipo_trabajo = 'A'; //  Textos teóricos (rápidos, no recalientan la máquina).
    } else {
        tipo_trabajo = 'B'; // Prácticas con imágenes (tardan más y recalientan la máquina).
    }

    colocar_trabajo_en_bandeja(id_dinosaurio);

    if (tipo_trabajo == 'B' && trabajos_b_seguidos == 2) {
        sem_wait(&prioridad_sem); // Espera si hay prioridad para A
        sem_wait(&enfriamiento_sem); // Espera a que termine el enfriamiento
        esperar_enfriamiento();
        trabajos_b_seguidos = 0;
    }

    usar_fotocopiadora(tipo_trabajo, id_dinosaurio);

    // Si es la segunda vez que realizo un trabajo B debo iniciar la espera y antes de liberar.

    pthread_exit(NULL);
}

int main() {
    // Inicializar semáforos
    sem_init(&fotocopiadora_sem, 0, 1);
    sem_init(&bandeja_sem, 0, 1);
    sem_init(&enfriamiento_sem, 0, 1);
    sem_init(&prioridad_sem, 0, 0); // Inicializado en 0 para bloquear trabajos B

    // Crear threads para los dinosaurios
    pthread_t threads[5];
    int ids[5] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, dinosaurio, &ids[i]);
    }

    // Esperar a que los threads terminen
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destruir semáforos
    sem_destroy(&fotocopiadora_sem);
    sem_destroy(&bandeja_sem);
    sem_destroy(&enfriamiento_sem);
    sem_destroy(&prioridad_sem);

    printf("Todos los dinosaurios han terminado sus trabajos.\n");
    printf("Total de trabajos realizados: %d\n", total_trabajos);
    printf("Fin del programa.\n");

    return 0;
}