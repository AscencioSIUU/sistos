/*
 * Ejercicio 2 - Memoria Virtual
 * Muestra direcciones de variables globales, locales y del heap,
 * luego imprime /proc/self/maps para analizar las regiones de memoria.
 *
 * Compilar: gcc -o ej2 ej2_memoria_virtual.c
 * Ejecutar: ./ej2
 */

#include <stdio.h>
#include <stdlib.h>

int globalVar = 100;

int main() {
    int localVar = 200;
    int *heapPtr = (int *)malloc(sizeof(int));

    if (heapPtr == NULL) {
        printf("Error al reservar memoria\n");
        return 1;
    }

    *heapPtr = 300;

    printf("=== Direcciones de variables ===\n");
    printf("globalVar (BSS/data): %p  -> valor: %d\n", (void *)&globalVar, globalVar);
    printf("localVar  (stack):    %p  -> valor: %d\n", (void *)&localVar,  localVar);
    printf("heapPtr   (heap):     %p  -> valor: %d\n", (void *)heapPtr,    *heapPtr);

    printf("\n=== /proc/self/maps ===\n\n");
    system("cat /proc/self/maps");

    free(heapPtr);
    return 0;
}
