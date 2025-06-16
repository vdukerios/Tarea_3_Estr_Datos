#include "processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

// Lee el archivo y construye el grafo en memoria
Graph* build_graph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    // Asumimos un máximo de 100 ciudades
    Node *nodes = malloc(sizeof(Node) * 100);
    int num_nodes = 0;

    char origin[32], destination[32];
    int distance;
    while (fscanf(file, "%31s %31s %d", origin, destination, &distance) == 3) {
        int origin_idx = -1, dest_idx = -1;
        // Buscar o agregar ciudad origen
        for (int i = 0; i < num_nodes; i++)
            if (strcmp(nodes[i].name, origin) == 0) origin_idx = i;
        if (origin_idx == -1) {
            strcpy(nodes[num_nodes].name, origin);
            nodes[num_nodes].edges = NULL;
            origin_idx = num_nodes++;
        }
        // Buscar o agregar ciudad destino
        for (int i = 0; i < num_nodes; i++)
            if (strcmp(nodes[i].name, destination) == 0) dest_idx = i;
        if (dest_idx == -1) {
            strcpy(nodes[num_nodes].name, destination);
            nodes[num_nodes].edges = NULL;
            dest_idx = num_nodes++;
        }
        // Agregar arista origen->destino
        Edge *new_edge = malloc(sizeof(Edge));
        new_edge->destination = dest_idx;
        new_edge->distance = distance;
        new_edge->next = nodes[origin_idx].edges;
        nodes[origin_idx].edges = new_edge;
        // Agregar arista destino->origen (grafo no dirigido)
        Edge *new_edge2 = malloc(sizeof(Edge));
        new_edge2->destination = origin_idx;
        new_edge2->distance = distance;
        new_edge2->next = nodes[dest_idx].edges;
        nodes[dest_idx].edges = new_edge2;
    }
    fclose(file);

    Graph *graph = malloc(sizeof(Graph));
    graph->nodes = nodes;
    graph->num_nodes = num_nodes;
    return graph;
}

// Busca el índice de una ciudad en el grafo
int find_city_index(Graph *graph, const char *city_name) {
    for (int i = 0; i < graph->num_nodes; i++)
        if (strcmp(graph->nodes[i].name, city_name) == 0)
            return i;
    return -1;
}

// Implementación del algoritmo de Dijkstra
int dijkstra(Graph *graph, int origin_idx, int dest_idx, int *path, int *path_length) {
    int n = graph->num_nodes;
    int *dist = malloc(sizeof(int) * n);
    int *prev = malloc(sizeof(int) * n);
    int *visited = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[origin_idx] = 0;

    for (int count = 0; count < n; count++) {
        int u = -1;
        for (int i = 0; i < n; i++)
            if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        if (u == -1 || dist[u] == INT_MAX) break;
        visited[u] = 1;

        for (Edge *e = graph->nodes[u].edges; e; e = e->next) {
            int v = e->destination;
            if (dist[u] + e->distance < dist[v]) {
                dist[v] = dist[u] + e->distance;
                prev[v] = u;
            }
        }
    }

    // Reconstruir el camino
    *path_length = 0;
    int u = dest_idx;
    if (dist[u] == INT_MAX) {
        free(dist); free(prev); free(visited);
        return -1;
    }
    while (u != -1) {
        path[(*path_length)++] = u;
        u = prev[u];
    }
    // Invertir el camino
    for (int i = 0; i < *path_length / 2; i++) {
        int tmp = path[i];
        path[i] = path[*path_length - i - 1];
        path[*path_length - i - 1] = tmp;
    }
    int total_distance = dist[dest_idx];
    free(dist); free(prev); free(visited);
    return total_distance;
}

// Libera la memoria utilizada por el grafo
void free_graph(Graph *graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        Edge *e = graph->nodes[i].edges;
        while (e) {
            Edge *tmp = e;
            e = e->next;
            free(tmp);
        }
    }
    free(graph->nodes);
    free(graph);
}

// Exporta el grafo como un archivo SVG simple
void export_graph_svg(Graph *graph, const char *output_filename) {
    FILE *out = fopen(output_filename, "w");
    if (!out) {
        fprintf(stderr, "Error: No se pudo crear el archivo SVG '%s'.\n", output_filename);
        return;
    }

    // Parámetros de dibujo
    int width = 800, height = 800;
    int radius = 300;
    int node_radius = 25;
    int cx = width / 2, cy = height / 2;

    // Calcular posiciones de los nodos en círculo
    double *pos_x = malloc(sizeof(double) * graph->num_nodes);
    double *pos_y = malloc(sizeof(double) * graph->num_nodes);
    for (int i = 0; i < graph->num_nodes; i++) {
        double angle = 2.0 * 3.141592653589793 * i / graph->num_nodes;
        pos_x[i] = cx + radius * cos(angle);
        pos_y[i] = cy + radius * sin(angle);
    }

    // Escribir encabezado SVG
    fprintf(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n", width, height);

    // Dibujar aristas (evitar duplicados)
    for (int i = 0; i < graph->num_nodes; i++) {
        Edge *edge = graph->nodes[i].edges;
        while (edge) {
            if (i < edge->destination) { // Solo una vez por arista
                fprintf(out,
                    "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#888\" stroke-width=\"2\" />\n",
                    pos_x[i], pos_y[i], pos_x[edge->destination], pos_y[edge->destination]);
                // Etiqueta de distancia
                double label_x = (pos_x[i] + pos_x[edge->destination]) / 2;
                double label_y = (pos_y[i] + pos_y[edge->destination]) / 2;
                fprintf(out,
                    "<text x=\"%.1f\" y=\"%.1f\" font-size=\"14\" fill=\"#333\" text-anchor=\"middle\" dy=\"-5\">%d km</text>\n",
                    label_x, label_y, edge->distance);
            }
            edge = edge->next;
        }
    }

    // Dibujar nodos
    for (int i = 0; i < graph->num_nodes; i++) {
        fprintf(out,
            "<circle cx=\"%.1f\" cy=\"%.1f\" r=\"%d\" fill=\"#4e79a7\" stroke=\"#222\" stroke-width=\"2\" />\n",
            pos_x[i], pos_y[i], node_radius);
        fprintf(out,
            "<text x=\"%.1f\" y=\"%.1f\" font-size=\"14\" fill=\"#fff\" text-anchor=\"middle\" dy=\"5\">%s</text>\n",
            pos_x[i], pos_y[i], graph->nodes[i].name);
    }

    fprintf(out, "</svg>\n");

    free(pos_x);
    free(pos_y);
    fclose(out);
}

