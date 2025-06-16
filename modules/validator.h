#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>

// Verifica si el archivo puede abrirse y tiene el formato correcto.
// Retorna true si es válido, false en caso contrario.
bool validate_file(const char *filename);

// Verifica si ambas ciudades existen en la lista de ciudades cargadas.
// Retorna true si ambas existen, false en caso contrario.
bool validate_cities(const char *origin, const char *destination, char **city_list, int num_cities);

// Verifica si los argumentos de línea de comandos son correctos.
// Retorna true si son válidos, false en caso contrario.
bool validate_arguments(int argc, char **argv);

#endif // VALIDATOR_H