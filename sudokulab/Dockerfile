# Usamos una imagen de Ubuntu para tener el entorno GCC
FROM ubuntu:latest

# Instalamos el compilador y utilidades necesarias
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    libomp-dev \
    procps \
    && rm -rf /var/lib/apt/lists/*

# Establecemos el directorio de trabajo
WORKDIR /usr/src/app

# Copiamos los archivos del laboratorio al contenedor
COPY . .

# Compilamos el programa
RUN gcc -o sudoku_validator SudokuValidator.c -lpthread -fopenmp

# Comando por defecto para mantener el contenedor abierto (modo interactivo)
CMD ["/bin/bash"]