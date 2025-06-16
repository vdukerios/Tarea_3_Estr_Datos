#ifndef PROCESSOR_H
#define PROCESSOR_H

// Estructura para representar una arista en el grafo
typedef struct Edge {
    int destination;
    int distance;
    struct Edge *next;
} Edge;

// Estructura para representar un nodo (ciudad) en el grafo
typedef struct Node {
    char name[32];
    Edge *edges;
} Node;

// Estructura para el grafo completo
typedef struct Graph {
    Node *nodes;
    int num_nodes;
} Graph;

// Lee el archivo y construye el grafo, devolviendo un puntero al grafo creado
// Retorna NULL si ocurre un error
Graph* build_graph(const char *filename);

// Busca el índice de una ciudad en el grafo, retorna -1 si no existe
int find_city_index(Graph *graph, const char *city_name);

// Ejecuta el algoritmo de Dijkstra para encontrar la ruta más corta
// Retorna la distancia total y llena el arreglo path con los índices de las ciudades en la ruta
// Retorna -1 si no hay ruta posible
int dijkstra(Graph *graph, int origin_idx, int dest_idx, int *path, int *path_length);

// Exporta el grafo como un archivo SVG
void export_graph_svg(Graph *graph, const char *output_filename);

// Libera la memoria utilizada por el grafo
void free_graph(Graph *graph);

#endif // PROCESSOR_H