# Ubuntu para clase de Sistemas Operativos
FROM ubuntu:22.04

# Evitar prompts interactivos durante la instalación
ENV DEBIAN_FRONTEND=noninteractive

# Actualizar e instalar herramientas esenciales para Sistemas Operativos
RUN apt-get update && apt-get install -y \
    # Compiladores y herramientas de build
    build-essential \
    gcc \
    g++ \
    make \
    cmake \
    # Ensamblador
    nasm \
    # Debugger
    gdb \
    # Análisis de memoria
    valgrind \
    # Trazado de llamadas al sistema
    strace \
    ltrace \
    # Herramientas de binarios
    binutils \
    # Editor de texto
    vim \
    nano \
    # Control de versiones
    git \
    # Manuales y documentación
    man-db \
    manpages-dev \
    manpages-posix \
    manpages-posix-dev \
    # Utilidades de red
    net-tools \
    iputils-ping \
    curl \
    wget \
    # Utilidades del sistema
    htop \
    tree \
    sudo \
    # Librería de hilos POSIX (pthread)
    libc6-dev \
    # Herramientas adicionales para procesos
    procps \
    psmisc \
    # Para trabajar con archivos
    file \
    xxd \
    # Locales
    locales \
    && rm -rf /var/lib/apt/lists/*

# Configurar locale
RUN locale-gen en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Crear usuario no-root (opcional, para prácticas de permisos)
RUN useradd -ms /bin/bash estudiante && \
    echo "estudiante:estudiante" | chpasswd && \
    adduser estudiante sudo

# Directorio de trabajo
WORKDIR /sistos

# Comando por defecto
CMD ["/bin/bash"]
