/*
 * Ejercicio 4 - Page Faults
 * Fases:
 *   1) Reserva 500 MB con malloc (sin usar la memoria)
 *   2) Espera 5 segundos (observar que la RAM no sube aun)
 *   3) Accede cada 4096 bytes (page faults reales ocurren aqui)
 *
 * Compilar: gcc -o ej4 ej4_page_faults.c
 * Ejecutar: ./ej4
 * Monitorear en otra terminal: vmstat 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MEM_SIZE   (500 * 1024 * 1024)  /* 500 MB */
#define PAGE_SIZE  4096

static void print_self_status(const char *fase) {
    printf("\n--- Estado de memoria [%s] ---\n", fase);
    system("grep -E 'VmRSS|VmSize|VmPeak' /proc/self/status");
    printf("-------------------------------------------\n\n");
}

int main() {
    printf("=== Fase 1: Reservando %d MB con malloc ===\n", MEM_SIZE / (1024 * 1024));
    char *mem = (char *)malloc(MEM_SIZE);

    if (mem == NULL) {
        printf("Error: no se pudo reservar memoria\n");
        return 1;
    }

    print_self_status("despues de malloc, antes de usar");

    printf("=== Fase 2: Esperando 5s sin tocar la memoria ===\n");
    printf("(Observa vmstat: sin page faults aun)\n");
    sleep(5);

    print_self_status("despues de esperar, antes de acceder");

    printf("=== Fase 3: Accediendo cada 4096 bytes (genera page faults) ===\n");
    int pages = MEM_SIZE / PAGE_SIZE;

    for (int i = 0; i < pages; i++) {
        mem[i * PAGE_SIZE] = 1;  /* primer acceso -> page fault */

        if (i % 10000 == 0) {
            printf("Progreso: %d%% (%d paginas)\n",
                   (i * 100) / pages, i);
        }
    }

    print_self_status("despues de acceder toda la memoria");

    printf("=== Fin: toda la memoria fue usada ===\n");

    free(mem);
    return 0;
}
