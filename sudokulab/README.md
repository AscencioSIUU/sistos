# Laboratorio 3 — Validador de Sudoku (pthreads + OpenMP)

Programa en C que valida soluciones de Sudoku 9×9 usando **pthreads** y **OpenMP**, ejecutado dentro de un contenedor **Docker**.

## Requisitos

- **Docker** instalado ([Instalar Docker](https://docs.docker.com/get-docker/))
- No se necesita GCC ni ningún compilador en tu máquina — todo corre dentro del contenedor.

## Estructura del proyecto

```
sudokulab/
├── Dockerfile            # Imagen Docker con Ubuntu + GCC + OpenMP
├── SudokuValidator.c     # Código fuente del validador
├── sudoku                # Archivo con solución de Sudoku (81 dígitos)
└── README.md             # Este archivo
```

## Construcción y ejecución

### 1. Construir la imagen Docker

```bash
cd sudokulab
docker build -t sudokulab .
```

### 2. Ejecutar en modo interactivo

```bash
docker run --cpus=4 -it sudokulab
```

> **Nota:** `--cpus=4` asigna 4 CPUs al contenedor para que OpenMP pueda crear threads en paralelo real. Esto es importante para observar los efectos de la paralelización.

Dentro del contenedor ya está compilado. Ejecuta:

```bash
./sudoku_validator sudoku
```

### 3. Ejecución directa (one-liner)

Si solo quieres ver el resultado sin entrar al contenedor:

```bash
docker run --cpus=4 -it sudokulab ./sudoku_validator sudoku
```

### 4. Recompilar dentro del contenedor

Si modificas el código y quieres recompilar manualmente dentro del contenedor:

```bash
gcc -o sudoku_validator SudokuValidator.c -lpthread -fopenmp
./sudoku_validator sudoku
```

## Probar en otra computadora

Solo se necesita **Docker** instalado. Luego:

```bash
# 1. Clonar el repositorio
git clone https://github.com/AscencioSIUU/sistos.git

# 2. Entrar al directorio del lab
cd sistos/sudokulab

# 3. Construir la imagen
docker build -t sudokulab .

# 4. Ejecutar
docker run --cpus=4 -it sudokulab ./sudoku_validator sudoku
```

Funciona en **Linux**, **macOS** y **Windows** (con Docker Desktop).

## Qué hace el programa

1. **Lee** el archivo de solución con `open()` + `mmap()` y carga los 81 dígitos en un arreglo 9×9.
2. **Valida sub-cuadrículas** (3×3) con OpenMP en paralelo.
3. **Primer `fork()` + `ps`**: muestra los LWPs (lightweight processes) del proceso padre durante la ejecución del pthread.
4. **Crea un `pthread`** que valida las **columnas** (con OpenMP dentro del thread).
5. Hace `pthread_join()` y muestra el thread ID del main.
6. **Valida filas** con OpenMP en paralelo.
7. Imprime si **la solución es válida o inválida**.
8. **Segundo `fork()` + `ps`**: muestra los LWPs antes de terminar (para comparar con el primer `ps`).

## Variantes de prueba (para las preguntas del reporte)

El código tiene líneas comentadas con `// EXPERIMENTO:` que puedes descomentar para probar diferentes configuraciones de OpenMP. Después de cada cambio, recompila dentro del contenedor:

```bash
# Editar el archivo (dentro del contenedor)
vi SudokuValidator.c    # o nano si prefieres

# Recompilar
gcc -o sudoku_validator SudokuValidator.c -lpthread -fopenmp

# Ejecutar
./sudoku_validator sudoku
```

### Experimentos disponibles

| Experimento | Línea a descomentar | Efecto |
|---|---|---|
| Limitar a 1 thread | `omp_set_num_threads(1)` en `main()` | Solo 1 LWP de OpenMP |
| schedule(dynamic) | `schedule(dynamic)` en los `#pragma` | Distribución dinámica de trabajo |
| 9 threads por función | `omp_set_num_threads(9)` en cada función | 9 threads OpenMP por validación |
| Nested parallelism | `omp_set_nested(1)` en cada función | Permite paralelismo anidado |

### Probar con un sudoku inválido

Para verificar que detecta errores, modifica un dígito del archivo `sudoku`:

```bash
# Dentro del contenedor, cambiar el primer dígito de 6 a 1
echo "124539187519728634837614295143865729958247361762391458371956842496182573285473916" > sudoku_invalido
./sudoku_validator sudoku_invalido
```

Debería imprimir: `*** La solucion del sudoku es INVALIDA ***`

## Formato del archivo de sudoku

- Una sola línea con **81 dígitos** (1-9), sin espacios ni separadores.
- Representa el sudoku fila por fila, de izquierda a derecha, de arriba a abajo.
- Ejemplo: `624539187519728634837614295...`
