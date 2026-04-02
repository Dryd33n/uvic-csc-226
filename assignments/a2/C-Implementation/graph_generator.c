#include <stdlib.h>
#include "graph_generator.h"

#define MAX_EDGE_WEIGHT 1000

void generate_random_graph(const int vertex_count, const double density,
                           EdgeListGraph **edge_list_graph,
                           AdjacencyMatrixGraph **adjacency_matrix_graph) {

    // determine number of edges graph will contain based on density percentage of the maximum possible edges
    const int max_edges = vertex_count * (vertex_count - 1);
    const int edge_count = (int)(density * max_edges);

    // the two data structures that the generator returns
    *edge_list_graph        = edge_list_graph_create(vertex_count, edge_count);
    *adjacency_matrix_graph = adjacency_matrix_graph_create(vertex_count);

    // add random edges until we reach the target edge count, yes i know its inefficient but its not being timed so i don't care <3
    int edges_added = 0;
    while (edges_added < edge_count) {
        const int from = rand() % vertex_count;
        const int to   = rand() % vertex_count;

        // skip self-loops
        if (from == to) continue;

        // skip if edge already exists
        if ((*adjacency_matrix_graph)->weights[from * vertex_count + to] != INFINITE_WEIGHT) continue;

        // random weight between 1 and MAX_EDGE_WEIGHT
        unsigned int weight = rand() % MAX_EDGE_WEIGHT + 1;

        //add edges to both representations
        edge_list_graph_add_edge(*edge_list_graph, from, to, weight);
        adjacency_matrix_graph_add_edge(*adjacency_matrix_graph, from, to, weight);
        edges_added++;
    }
}
