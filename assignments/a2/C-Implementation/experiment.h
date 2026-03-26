#pragma once
#include "graph.h"

int *generate_source_vertices(int vertex_count, int k);
unsigned int **dijkstra_k_sources(const EdgeListGraph *graph, int *sources, int k);
