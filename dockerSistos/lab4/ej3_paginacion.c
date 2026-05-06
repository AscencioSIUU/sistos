/*
 * Ejercicio 3 - Simulacion de Paginacion
 * Reserva un arreglo grande y accede cada 4096 bytes (tamano de pagina).
 * Monitorea con: top o htop en otra terminal.
 *
 * Compilar: gcc -o ej3 ej3_paginacion.c
 * Ejecutar: ./ej3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE (50 * 1024 * 1024)  /* 50 MB */
#define PAGE_SIZE  4096

int main() {
    printf("Reservando %d MB...\n", ARRAY_SIZE / (1024 * 1024));
    char *arr = (char *)malloc(ARRAY_SIZE);

    if (arr == NULL) {
        printf("Error al reservar memoria\n");
        return 1;
    }

    printf("Memoria reservada. Iniciando accesos cada %d bytes (1 pagina)...\n", PAGE_SIZE);
    printf("Monitorea con 'top' o 'htop' en otra terminal.\n\n");

    int pages = ARRAY_SIZE / PAGE_SIZE;
    int contador = 0;

    for (int i = 0; i < pages; i++) {
        arr[i * PAGE_SIZE] = (char)(i % 256);
        contador++;

        if (contador % 1000 == 0) {
            printf("Paginas accedidas: %d / %d\n", contador, pages);
            usleep(50000);  /* 50ms de retardo cada 1000 paginas */
        }
    }

    printf("\nTotal de paginas accedidas: %d\n", contador);
    printf("Memoria total tocada: %d MB\n", (contador * PAGE_SIZE) / (1024 * 1024));

    free(arr);
    return 0;
}
