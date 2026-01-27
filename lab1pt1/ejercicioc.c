#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char*argv[]) {
    // Verificar que se reciban exactamente 2 argumentos
    if (argc != 3) {
        printf("Uso: %s <archivo_fuente> <archivo_destino>\n", argv[0]);
        return 1;
    }
    
    // Abrir archivo fuente para lectura
    int fd_fuente = open(argv[1], O_RDONLY);
    if (fd_fuente == -1) {
        perror("Error al abrir archivo fuente");
        return 1;
    }
    
    // Crear/abrir archivo destino para escritura
    int fd_destino = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_destino == -1) {
        perror("Error al crear archivo destino");
        close(fd_fuente);
        return 1;
    }
    
    // Buffer para leer datos
    char buffer[1024];
    ssize_t bytes_leidos;
    
    // Copiar contenido del archivo fuente al destino
    while ((bytes_leidos = read(fd_fuente, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_escritos = write(fd_destino, buffer, bytes_leidos);
        if (bytes_escritos != bytes_leidos) {
            perror("Error al escribir en archivo destino");
            close(fd_fuente);
            close(fd_destino);
            return 1;
        }
    }
    
    // Verificar si hubo error en la lectura
    if (bytes_leidos == -1) {
        perror("Error al leer archivo fuente");
        close(fd_fuente);
        close(fd_destino);
        return 1;
    }
    
    // Cerrar archivos
    close(fd_fuente);
    close(fd_destino);
    
    printf("Archivo copiado exitosamente de %s a %s\n", argv[1], argv[2]);
    return 0;
}
