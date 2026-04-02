#include <stdlib.h>
#include "experiment.h"
#include "dijkstra.h"

int *generate_source_vertices(const int vertex_count, const int k) {
    int *sources    = malloc((size_t)k * sizeof(int));
    int  sources_found = 0;

    while (sources_found < k) {
        int candidate = rand() % vertex_count;

        // check for duplicates
        int duplicate = 0;
        for (int i = 0; i < sources_found; i++) {
            if (sources[i] == candidate) { duplicate = 1; break; }
        }

        if (!duplicate) sources[sources_found++] = candidate;
    }

    return sources;
}

unsigned int **dijkstra_k_sources(const EdgeListGraph *graph, const int *sources, int k) {
    unsigned int **distances = malloc((size_t)k * sizeof(unsigned int *));

    // run dijkstra from each source vertex
    for (int i = 0; i < k; i++)
        distances[i] = dijkstra(graph, sources[i]);

    return distances;
}
