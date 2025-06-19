# Tarea 3 - Estructura de Datos

**Valentin Duke**
**Arturo Figueroa**

Este proyecto implementa un programa en C que permite cargar un grafo de ciudades y rutas desde un archivo, validar los datos, encontrar la ruta más corta entre dos ciudades usando Dijkstra y exportar el grafo a SVG.

## Compilación

Para compilar el programa, usa el siguiente comando en la terminal:

```
make
```

Esto generará el ejecutable `tarea3`.

## Ejecución

Para ejecutar el programa, usa:

```
./tarea3 rutas.txt <ORIGEN> <DESTINO>
```

Por ejemplo:

```
./tarea3 rutas.txt SCL NYC
```

## Estructura del Proyecto

- `main.c`: Función principal, orquesta la validación, construcción del grafo, búsqueda de ruta y exportación.
- `modules/validator.c` y `modules/validator.h`: Validación de argumentos, archivo y ciudades.
- `modules/processor.c` y `modules/processor.h`: Construcción y manejo del grafo, Dijkstra, exportación SVG.

## Descripción de los procedimientos principales

### main.c

- **main**: Controla el flujo del programa. Valida argumentos, construye el grafo, valida ciudades, ejecuta Dijkstra y muestra la ruta óptima.

### modules/validator.c

- **validate_arguments**: Verifica que los argumentos de línea de comandos sean correctos.
- **validate_file**: Verifica que el archivo de rutas exista y tenga el formato correcto.
- **validate_cities**: Verifica que las ciudades de origen y destino existan en la lista de ciudades.

### modules/processor.c

- **build_graph**: Lee el archivo de rutas y construye el grafo en memoria, agregando nodos y aristas.
- **find_city_index**: Busca el índice de una ciudad en el grafo.
- **dijkstra**: Implementa el algoritmo de Dijkstra para encontrar la ruta más corta entre dos ciudades. Devuelve la distancia total y el camino.
- **export_graph_svg**: Exporta el grafo a un archivo SVG, dibujando nodos y aristas.
- **free_graph**: Libera toda la memoria utilizada por el grafo.

## Limpieza

Para eliminar el ejecutable generado:

```
make clean
```

---

