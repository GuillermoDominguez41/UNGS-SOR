#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Semaforos
sem_t fotocopiadora_sem; // Controla el acceso exclusivo a la fotocopiadora
sem_t bandeja_sem; // Controla el acceso exclusivo a la bandeja
sem_t prioridad_TipoA_sem; // Controla la prioridad A sobre B

// Variables Globales
int trabajos_b_seguidos = 0;
int total_trabajos = 0;

// PRIMITIVAS (DEFINIDAS EN EL ENUNCIADO)
void esperar_enfriamiento() {
    printf("La fotocopiadora necesita enfriarse...\n");
    sleep(5);
    printf("La fotocopiadora está lista para usar\n");
}

void imprimir_estado(int id_dinosaurio, char tipo_trabajo) {
    printf("-------------------------------\n");
    printf("Dinosaurio usando: %d\n", id_dinosaurio);
    printf("Tipo de trabajo: %c\n", tipo_trabajo);
    printf("Enfriar al finalizar: %s\n", trabajos_b_seguidos == 2 ? "Sí" : "No");
    printf("-------------------------------\n");
}

// Simula el uso de la fotocopiadora
void usar_fotocopiadora(char tipo_trabajo, int id_dinosaurio) {
    sem_wait(&fotocopiadora_sem);

    if (tipo_trabajo == 'A') {
        trabajos_b_seguidos = 0; // Reinicia el contador de trabajos B consecutivos
        total_trabajos++;
        imprimir_estado(id_dinosaurio, tipo_trabajo);
        sleep(1); // Simula el tiempo de fotocopiado para A
    } else {
        trabajos_b_seguidos++;
        total_trabajos++;

        imprimir_estado(id_dinosaurio, tipo_trabajo);
        sleep(3); // Simula el tiempo de fotocopiado para B

     // Si hay 2 trabajos B seguidos, esperar enfriamiento}
        if(trabajos_b_seguidos == 2)
            esperar_enfriamiento(); 

     // Si hay más de 2 trabajos B seguidos, esperar prioridad para A
        if (trabajos_b_seguidos > 2) {
            int intentos = 5;
            while (intentos > 0) {
                if (sem_trywait(&prioridad_TipoA_sem) == 0) {
                    // Se desbloqueó la prioridad porque llegó un trabajo de tipo A
                    break;
                }
                sleep(1); // Esperar 1 segundo antes de intentar de nuevo
                intentos--;
            }

            if (intentos == 0) {
                // Si se agotaron los intentos, desbloquear manualmente la prioridad
                printf("Tiempo de espera agotado. Continuando con trabajos tipo B.\n");
            }
        }
    } 
    
    printf("Dinosaurio %d terminó de usar la fotocopiadora\n", id_dinosaurio);
    sem_post(&fotocopiadora_sem);
}

void colocar_trabajo_en_bandeja(char tipo_trabajo, int id_dinosaurio) {
    sem_wait(&bandeja_sem);
    printf("Dinosaurio %d coloco el trabajo de tipo %c en la bandeja\n", id_dinosaurio, tipo_trabajo);
    sleep(1);
    sem_post(&bandeja_sem);
}

// Definir el tipo de trabajo (A o B) aleatoriamente
char definir_tipo_trabajo() {
    if (rand() % 100 < 50) {
        return 'A'; //  Textos teóricos (rápidos, no recalientan la máquina).
    } else {
        return 'B'; // Prácticas con imágenes (tardan más y recalientan la máquina).
    }
}

// Funcion thread del dinosaurio
void *dinosaurio(void *arg) { 
    int id_dinosaurio = *(int *)arg;
    char tipo_trabajo = definir_tipo_trabajo();

    colocar_trabajo_en_bandeja(tipo_trabajo, id_dinosaurio);
    usar_fotocopiadora(tipo_trabajo, id_dinosaurio);
    pthread_exit(NULL);
}

int main() {
    // Inicializar semáforos
    sem_init(&fotocopiadora_sem, 0, 1);
    sem_init(&bandeja_sem, 0, 1);
    sem_init(&prioridad_TipoA_sem, 0, 0);

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
    sem_destroy(&prioridad_TipoA_sem);

    printf("Todos los dinosaurios han terminado sus trabajos.\n");
    printf("Total de trabajos realizados: %d\n", total_trabajos);
    printf("Fin del programa.\n");

    return 0;
}