# Laboratorio 04 – Sistemas Operativos (Resumen en Markdown)

## 🎯 Objetivo

Aplicar conceptos fundamentales de sistemas operativos:

* Procesos y su terminación
* Relación padre-hijo
* Cambio de contexto
* Comunicación entre procesos (IPC):

  * Memoria compartida
  * Pipes (ordinarios y nombrados)

---

## 📦 Entregables

Debes entregar:

### 1. Documento PDF

* Respuestas completas y bien explicadas
* Análisis detallado (no solo resultados)
* Estructura formal (nivel ingeniería)
* Capturas de pantalla como evidencia

### 2. Código fuente

* Archivos bien comentados
* Identificados correctamente

### 3. Archivo comprimido (.zip)

* Incluir todo el código

### 4. Instrucciones

* Comandos de compilación
* Comandos de ejecución

---

## ⚙️ Requisitos

* Máquina virtual con Linux

---

# 🧪 Ejercicio 1: Uso de memoria

## a) Analizar mapa de memoria

Ejecutar:

```
cat /proc/self/maps
```

Responder:

* ¿Qué regiones existen?

  * heap
  * stack
  * text
  * librerías compartidas
* Diferencia entre direcciones iniciales y finales
* ¿Por qué no son contiguas?

## b) Información de memoria

Ejecutar:

```
cat /proc/meminfo
```

Responder:

* Indicadores de uso real de memoria
* Diferencia entre:

  * MemFree
  * MemAvailable

---

# 🧪 Ejercicio 2: Memoria virtual

## a) Ejecutar programa en C

* Observar:

  * Variables globales
  * Variables locales
  * Memoria dinámica (malloc)
* Analizar `/proc/self/maps` generado por el programa

## b) Repetir ejecución

Responder:

* ¿Cambian las direcciones?
* ¿Qué mecanismo lo explica?

  * (ASLR)
* ¿Qué ventaja tiene?

## c) Relación de direcciones

Ubicar en el mapa:

* [heap]
* [stack]
* ejecutable

Relacionar con:

* globalVar
* localVar
* heapPtr

Responder:

* ¿Dónde está cada tipo de variable?
* ¿Cómo identificar cada región?
* ¿Qué patrón tienen las direcciones?

---

# 🧪 Ejercicio 3: Simulación de paginación

## a) Modificar programa

* Crear arreglo grande (MBs)
* Acceder cada 4096 bytes

## b) Agregar

* Contador
* Retardo entre accesos

## c) Monitorear

Ejecutar en otra terminal:

```
top
```

o

```
htop
```

Responder:

* ¿Cambia el uso de memoria?
* Relación entre 4096 bytes y tamaño de página

## d) Explicación

* Relación con páginas y frames

---

# 🧪 Ejercicio 4: Page faults

## a) Programa en C

Reservar memoria (ej: 500MB)

### Fases:

1. Reservar memoria (malloc)
2. Esperar sin usar memoria
3. Acceder cada 4096 bytes

## b) Monitoreo

Ejecutar:

```
vmstat 1
```

## c) Análisis

Responder:

* Diferencia entre reservar y usar memoria
* ¿Qué pasa al escribir por primera vez?
* ¿Qué son page faults?
* ¿Por qué no se usa toda la memoria inmediatamente?
* Relación con páginas (4096 bytes)

---

# 🧠 Conceptos clave que debes dominar

* Memoria virtual
* Segmentos de memoria (heap, stack, text)
* ASLR
* Paginación
* Page faults
* Lazy allocation
* Tamaño de página (≈ 4KB)

---

# ✅ Tips para el laboratorio

* Siempre explica el *por qué*, no solo el *qué*
* Relaciona resultados con teoría
* Usa capturas claras
* Corre múltiples veces los programas
* Compara resultados

---

# 🚀 Resultado esperado

Debes ser capaz de:

* Interpretar mapas de memoria
* Entender cómo el SO asigna memoria
* Explicar paginación y fallos de página
* Analizar comportamiento real del sistema

---
 
Codigo proporcionado

```
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
printf("Direccion de globalVar: %p\n", (void *)&globalVar);
printf("Direccion de localVar: %p\n", (void *)&localVar);
printf("Direccion de heapPtr: %p\n", (void *)heapPtr);
printf("\nContenido de /proc/self/maps:\n\n");
system("cat /proc/self/maps");
free(heapPtr);
return 0;
}
```