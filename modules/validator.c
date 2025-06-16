#include "validator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Verifica si el archivo puede abrirse y si cada línea tiene al menos 3 elementos (origen, destino, distancia)
bool validate_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: No se pudo abrir el archivo '%s'.\n", filename);
        return false;
    }

    char line[256];
    int line_num = 1;
    while (fgets(line, sizeof(line), file)) {
        char origin[32], destination[32];
        int distance;
        int read = sscanf(line, "%31s %31s %d", origin, destination, &distance);
        if (read != 3 || distance <= 0) {
            fprintf(stderr, "Error: Formato incorrecto en la línea %d del archivo.\n", line_num);
            fclose(file);
            return false;
        }
        line_num++;
    }
    fclose(file);
    return true;
}

// Verifica si ambas ciudades existen en la lista de ciudades cargadas
bool validate_cities(const char *origin, const char *destination, char **city_list, int num_cities) {
    bool origin_found = false, destination_found = false;
    for (int i = 0; i < num_cities; i++) {
        if (strcmp(city_list[i], origin) == 0) origin_found = true;
        if (strcmp(city_list[i], destination) == 0) destination_found = true;
    }
    if (!origin_found || !destination_found) {
        fprintf(stderr, "Error: La ciudad '%s' o '%s' no se encuentra en la lista de ciudades.\n", origin, destination);
        return false;
    }
    return true;
}

// Verifica si los argumentos de línea de comandos son correctos
bool validate_arguments(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <archivo_rutas> <ciudad_origen> <ciudad_destino>\n", argv[0]);
        return false;
    }
    // Se puede agregar validación adicional si es necesario
    return true;
}