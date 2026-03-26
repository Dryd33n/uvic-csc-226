#pragma once
#include "graph.h"

void generate_random_graph(int vertex_count, double density,
                           EdgeListGraph **edge_list_graph,
                           AdjacencyMatrixGraph **adjacency_matrix_graph);
