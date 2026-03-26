#include <stdlib.h>
#include <string.h>
#include "graph.h"

// Adjacency list graph using likedlists

EdgeListGraph *edge_list_graph_create(int vertex_count, int initial_edge_capacity) {
    EdgeListGraph *graph = malloc(sizeof *graph);
    graph->vertex_count  = vertex_count;
    graph->edge_count    = 0;
    graph->edge_capacity = initial_edge_capacity;
    graph->head          = malloc((size_t)vertex_count * sizeof(int));
    graph->edges         = malloc((size_t)initial_edge_capacity * sizeof(Edge));

    memset(graph->head, -1, (size_t)vertex_count * sizeof(int));
    return graph;
}

void edge_list_graph_add_edge(EdgeListGraph *graph, int from, int to, unsigned int weight) {
    if (graph->edge_count == graph->edge_capacity) {
        graph->edge_capacity *= 2;
        graph->edges = realloc(graph->edges, (size_t)graph->edge_capacity * sizeof(Edge));
    }

    graph->edges[graph->edge_count] = (Edge){
        .destination     = to,
        .weight          = weight,
        .next_edge_index = graph->head[from],
    };
    graph->head[from] = graph->edge_count++;
}

void edge_list_graph_free(EdgeListGraph *graph) {
    free(graph->head);
    free(graph->edges);
    free(graph);
}

// Adjacency matrix graph

AdjacencyMatrixGraph *adjacency_matrix_graph_create(int vertex_count) {
    AdjacencyMatrixGraph *graph = malloc(sizeof *graph);
    graph->vertex_count         = vertex_count;
    graph->weights              = malloc((size_t)vertex_count * (size_t)vertex_count * sizeof(unsigned int));

    // initialize all edges to infinite weight, diagonal to 0
    for (int i = 0; i < vertex_count; i++)
        for (int j = 0; j < vertex_count; j++)
            graph->weights[i * vertex_count + j] = (i == j) ? 0 : INFINITE_WEIGHT;
    return graph;
}

void adjacency_matrix_graph_add_edge(AdjacencyMatrixGraph *graph, int from, int to, unsigned int weight) {
    unsigned int *cell = &graph->weights[from * graph->vertex_count + to];
    if (weight < *cell) *cell = weight;
}

void adjacency_matrix_graph_free(AdjacencyMatrixGraph *graph) {
    free(graph->weights);
    free(graph);
}
