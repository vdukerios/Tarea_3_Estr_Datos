#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/validator.h"
#include "modules/processor.h"

int main(int argc, char **argv) {
    // Validar argumentos de línea de comandos
    if (!validate_arguments(argc, argv)) {
        return 1;
    }

    const char *filename = argv[1];
    const char *origin = argv[2];
    const char *destination = argv[3];

    // Validar archivo de entrada
    if (!validate_file(filename)) {
        return 1;
    }

    // Construir el grafo a partir del archivo
    Graph *graph = build_graph(filename);
    if (!graph) {
        fprintf(stderr, "Error: No se pudo construir el grafo.\n");
        return 1;
    }
    
    // Exportar el grafo a un archivo SVG
    export_graph_svg(graph, "graph_output.svg");

    // Validar que las ciudades existan en el grafo
    int origin_idx = find_city_index(graph, origin);
    int dest_idx = find_city_index(graph, destination);

    // Crear lista de nombres de ciudades para validación
    char **city_list = malloc(sizeof(char*) * graph->num_nodes);
    for (int i = 0; i < graph->num_nodes; i++) {
        city_list[i] = graph->nodes[i].name;
    }
    if (!validate_cities(origin, destination, city_list, graph->num_nodes)) {
        free(city_list);
        free_graph(graph);
        return 1;
    }
    free(city_list);

    // Ejecutar Dijkstra para encontrar la ruta óptima
    int path[graph->num_nodes];
    int path_length = 0;
    int total_distance = dijkstra(graph, origin_idx, dest_idx, path, &path_length);

    if (total_distance == -1) {
        printf("No existe una ruta entre %s y %s.\n", origin, destination);
        free_graph(graph);
        return 1;
    }

    // Mostrar la ruta óptima
    printf("Ruta óptima: ");
    for (int i = 0; i < path_length; i++) {
        printf("%s", graph->nodes[path[i]].name);
        if (i < path_length - 1) printf(" -> ");
    }
    printf("\nDistancia total: %d km\n", total_distance);

    free_graph(graph);
    return 0;
}