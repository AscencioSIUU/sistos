/*
 * SudokuValidator.c
 * Laboratorio 3 - Sistemas Operativos (CC3064)
 * Validador de soluciones de Sudoku 9x9 usando pthreads y OpenMP.
 *
 * Uso: ./sudoku_validator <archivo_sudoku>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <omp.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

/* Arreglo global para que sea accesible por todos los threads */
int sudoku[9][9];

/*
 * resultados[0] = 1 si todas las filas son válidas, 0 si no
 * resultados[1] = 1 si todas las columnas son válidas, 0 si no
 * resultados[2] = 1 si todas las sub-cuadrículas son válidas, 0 si no
 */
int resultados[3] = {1, 1, 1};

/* ====================================================================
 * Funciones de validación
 * ==================================================================== */

/*
 * Valida que cada fila contenga los dígitos 1-9 sin repetir.
 * Paralelizado con OpenMP.
 */
void validar_filas() {
    // EXPERIMENTO: descomentar para fijar el número de threads a 9
    // omp_set_num_threads(9);

    // EXPERIMENTO: descomentar para habilitar nested parallelism
    // omp_set_nested(1);

    int valido = 1;
    #pragma omp parallel for /* schedule(dynamic) */
    for (int i = 0; i < 9; i++) {
        int check[10] = {0};
        for (int j = 0; j < 9; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || check[num] == 1) {
                valido = 0;
            }
            check[num] = 1;
        }
        printf("  [Filas]    Thread %d validando fila %d\n", omp_get_thread_num(), i);
    }
    resultados[0] = valido;
}

/*
 * Valida que cada columna contenga los dígitos 1-9 sin repetir.
 * Paralelizado con OpenMP.
 */
void validar_columnas() {
    // EXPERIMENTO: descomentar para fijar el número de threads a 9
    // omp_set_num_threads(9);

    // EXPERIMENTO: descomentar para habilitar nested parallelism
    // omp_set_nested(1);

    int valido = 1;
    #pragma omp parallel for /* schedule(dynamic) */
    for (int j = 0; j < 9; j++) {
        int check[10] = {0};
        for (int i = 0; i < 9; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || check[num] == 1) {
                valido = 0;
            }
            check[num] = 1;
        }
        printf("  [Columnas] Thread %d validando columna %d\n", omp_get_thread_num(), j);
    }
    resultados[1] = valido;
}

/*
 * Valida una sub-cuadrícula de 3x3 cuya esquina superior izquierda
 * está en (fila, col). Retorna 1 si es válida, 0 si no.
 */
int validar_subcuadricula(int fila, int col) {
    int check[10] = {0};
    for (int i = fila; i < fila + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || check[num] == 1) {
                return 0;
            }
            check[num] = 1;
        }
    }
    return 1;
}

/*
 * Valida las 9 sub-cuadrículas de 3x3 del sudoku.
 * Las esquinas superiores izquierdas están en las posiciones (i,j)
 * con i,j pertenecientes a {0, 3, 6}.
 * Paralelizado con OpenMP.
 */
void validar_subcuadriculas() {
    // EXPERIMENTO: descomentar para fijar el número de threads a 9
    // omp_set_num_threads(9);

    // EXPERIMENTO: descomentar para habilitar nested parallelism
    // omp_set_nested(1);

    int valido = 1;
    #pragma omp parallel for collapse(2) /* schedule(dynamic) */
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            if (!validar_subcuadricula(i, j)) {
                valido = 0;
            }
            printf("  [Subcuad]  Thread %d validando subcuadricula (%d,%d)\n",
                   omp_get_thread_num(), i, j);
        }
    }
    resultados[2] = valido;
}

/* ====================================================================
 * Función wrapper para pthread (validación de columnas)
 * ==================================================================== */

void* thread_validar_columnas(void* arg) {
    (void)arg;
    printf("\n--- Inicio validacion de columnas (pthread) ---\n");
    printf("  Thread ID (tid) del pthread: %ld\n", syscall(SYS_gettid));

    validar_columnas();

    printf("--- Fin validacion de columnas (pthread) ---\n\n");
    pthread_exit(0);
}

/* ====================================================================
 * Función auxiliar: fork + exec de ps -p <pid> -lLf
 * ==================================================================== */

void ejecutar_ps(pid_t pid_padre) {
    char pid_str[20];
    snprintf(pid_str, sizeof(pid_str), "%d", pid_padre);

    pid_t pid = fork();
    if (pid == 0) {
        /* Proceso hijo: ejecuta ps */
        execlp("ps", "ps", "-p", pid_str, "-lLf", NULL);
        perror("execlp fallo");
        exit(1);
    } else if (pid > 0) {
        /* Proceso padre: espera al hijo */
        waitpid(pid, NULL, 0);
    } else {
        perror("fork fallo");
    }
}

/* ====================================================================
 * main
 * ==================================================================== */

int main(int argc, char *argv[]) {
    // EXPERIMENTO: descomentar para limitar OpenMP a 1 thread
    // omp_set_num_threads(1);

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_sudoku>\n", argv[0]);
        return 1;
    }

    /* --- Paso 1: Abrir el archivo con open() y mapearlo con mmap() --- */
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error en fstat");
        close(fd);
        return 1;
    }

    char *data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("Error en mmap");
        close(fd);
        return 1;
    }

    /* --- Paso 2: Copiar los 81 dígitos al arreglo bidimensional --- */
    printf("=== Cargando sudoku desde archivo: %s ===\n\n", argv[1]);
    for (int i = 0; i < 81; i++) {
        sudoku[i / 9][i % 9] = data[i] - '0';
    }

    /* Liberar el mapeo y cerrar el archivo */
    munmap(data, sb.st_size);
    close(fd);

    /* Imprimir la cuadrícula cargada */
    printf("Cuadricula cargada:\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    /* --- Paso 3: Validar sub-cuadrículas --- */
    printf("--- Validando sub-cuadriculas ---\n");
    validar_subcuadriculas();
    printf("  Resultado sub-cuadriculas: %s\n\n",
           resultados[2] ? "VALIDO" : "INVALIDO");

    /* --- Paso 4: Obtener PID y primer fork + ps --- */
    pid_t pid_padre = getpid();
    printf("PID del proceso padre: %d\n", pid_padre);
    printf("Thread ID (tid) del main: %ld\n\n", syscall(SYS_gettid));

    printf("=== Primer ps (durante validacion de columnas con pthread) ===\n");

    pid_t pid1 = fork();
    if (pid1 == 0) {
        /* Hijo: ejecuta ps para mostrar LWPs durante el pthread */
        char pid_str[20];
        snprintf(pid_str, sizeof(pid_str), "%d", pid_padre);
        execlp("ps", "ps", "-p", pid_str, "-lLf", NULL);
        perror("execlp fallo");
        exit(1);
    }

    /* --- Paso 5: Padre crea pthread para validar columnas --- */
    pthread_t hilo_columnas;
    pthread_create(&hilo_columnas, NULL, thread_validar_columnas, NULL);
    pthread_join(hilo_columnas, NULL);

    printf("Thread ID (tid) del main despues de join: %ld\n", syscall(SYS_gettid));
    printf("  Resultado columnas: %s\n\n",
           resultados[1] ? "VALIDO" : "INVALIDO");

    /* Esperar al hijo que ejecuta ps */
    waitpid(pid1, NULL, 0);

    /* --- Paso 6: Validar filas --- */
    printf("--- Validando filas ---\n");
    validar_filas();
    printf("  Resultado filas: %s\n\n",
           resultados[0] ? "VALIDO" : "INVALIDO");

    /* --- Paso 7: Resultado final --- */
    if (resultados[0] && resultados[1] && resultados[2]) {
        printf("*** La solucion del sudoku es VALIDA ***\n\n");
    } else {
        printf("*** La solucion del sudoku es INVALIDA ***\n\n");
    }

    /* --- Paso 8: Segundo fork + ps (antes de terminar) --- */
    printf("=== Segundo ps (antes de terminar el programa) ===\n");
    ejecutar_ps(pid_padre);

    return 0;
}