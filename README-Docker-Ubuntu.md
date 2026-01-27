# 🐧 Ubuntu con Docker para Sistemas Operativos

Este setup te permite usar Ubuntu con todas las herramientas necesarias para la clase de Sistemas Operativos, con persistencia de datos.

## 📋 Requisitos Previos

- [Docker Desktop](https://www.docker.com/products/docker-desktop/) instalado
- Docker Compose (viene incluido con Docker Desktop)

## 🚀 Comandos Básicos

### Primera vez - Construir y levantar

```bash
# Construir la imagen (solo la primera vez o cuando cambies el Dockerfile)
docker-compose build

# Levantar el contenedor
docker-compose up -d
```

### Entrar al contenedor

```bash
# Acceder como root
docker-compose exec ubuntu-sistos bash

# Acceder como usuario estudiante
docker-compose exec -u estudiante ubuntu-sistos bash
```

### Comandos del día a día

```bash
# Ver si el contenedor está corriendo
docker-compose ps

# Detener el contenedor (NO borra datos)
docker-compose stop

# Levantar el contenedor detenido
docker-compose start

# Ver logs
docker-compose logs

# Reiniciar
docker-compose restart
```

### Eliminar (⚠️ Cuidado)

```bash
# Detener y eliminar contenedor (los archivos en /sistos se mantienen)
docker-compose down

# Eliminar TODO incluyendo volúmenes (⚠️ pierdes configs del home)
docker-compose down -v
```

## 🛠️ Herramientas Instaladas

| Herramienta | Descripción | Ejemplo de uso |
|-------------|-------------|----------------|
| `gcc` | Compilador de C | `gcc programa.c -o programa` |
| `g++` | Compilador de C++ | `g++ programa.cpp -o programa` |
| `make` | Automatización de builds | `make` |
| `gdb` | Debugger | `gdb ./programa` |
| `valgrind` | Detector de memory leaks | `valgrind ./programa` |
| `strace` | Traza llamadas al sistema | `strace ./programa` |
| `ltrace` | Traza llamadas a librerías | `ltrace ./programa` |
| `nasm` | Ensamblador | `nasm -f elf64 codigo.asm` |
| `man` | Manuales | `man fork`, `man 2 open` |

## 📁 Estructura de Directorios

```
/sistos/              ← Tu directorio de trabajo (sincronizado con tu Mac)
├── jan21/            ← Tus archivos existentes
├── lab1pt1/
├── Dockerfile
├── docker-compose.yml
└── README-Docker-Ubuntu.md

/home/estudiante/     ← Home del usuario (persistente en volumen Docker)
```

## 📝 Ejemplos de Uso

### Compilar y ejecutar un programa en C

```bash
# Dentro del contenedor
cd /sistos/jan21
gcc Ej_print.c -o Ej_print
./Ej_print
```

### Compilar con flags de debug

```bash
gcc -g -Wall programa.c -o programa
```

### Ver llamadas al sistema de un programa

```bash
strace ./programa
```

### Usar el debugger

```bash
gcc -g programa.c -o programa
gdb ./programa
# Dentro de gdb:
# (gdb) break main
# (gdb) run
# (gdb) next
# (gdb) print variable
# (gdb) quit
```

### Verificar memory leaks

```bash
gcc -g programa.c -o programa
valgrind --leak-check=full ./programa
```

### Consultar manuales de llamadas al sistema

```bash
man fork          # Manual de fork()
man 2 open        # Manual de open() (sección 2 = syscalls)
man 3 printf      # Manual de printf() (sección 3 = funciones de librería)
```

## 🔄 Flujo de Trabajo Recomendado

1. **Edita** tus archivos en VS Code (en tu Mac)
2. **Compila y ejecuta** dentro del contenedor Docker
3. Los cambios se sincronizan automáticamente

```bash
# Terminal 1: VS Code abierto en /sistos
# Terminal 2: 
docker-compose exec ubuntu-sistos bash
cd /sistos/lab1pt1
gcc ejerciciob.c -o ejerciciob
./ejerciciob
```

## ⚡ Alias Útiles (opcional)

Agrega estos alias a tu `~/.zshrc` o `~/.bashrc` en tu Mac:

```bash
# Entrar rápido al contenedor
alias sistos='docker-compose -f ~/Documents/git_u/2026/sistos/docker-compose.yml exec ubuntu-sistos bash'

# Levantar el contenedor
alias sistos-up='docker-compose -f ~/Documents/git_u/2026/sistos/docker-compose.yml up -d'

# Detener el contenedor
alias sistos-stop='docker-compose -f ~/Documents/git_u/2026/sistos/docker-compose.yml stop'
```

## 🐛 Solución de Problemas

### El contenedor no inicia
```bash
docker-compose logs
docker-compose down
docker-compose up -d
```

### Reconstruir la imagen
```bash
docker-compose build --no-cache
docker-compose up -d
```

### Ver uso de recursos
```bash
docker stats sistos-ubuntu
```

### Permisos de archivos
Si tienes problemas de permisos, usa root:
```bash
docker-compose exec ubuntu-sistos bash
# Ya estás como root
```

## 📚 Recursos Adicionales

- [The Linux Command Line](https://linuxcommand.org/tlcl.php) - Libro gratuito
- [GNU C Library Manual](https://www.gnu.org/software/libc/manual/)
- [Linux System Call Table](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md)
