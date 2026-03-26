#pragma once
#include <limits.h>

#define INFINITE_WEIGHT UINT_MAX

typedef struct edge {
    int          destination;
    unsigned int weight;
    int          next_edge_index;
} Edge;

typedef struct {
    int   vertex_count;
    int   edge_count;
    int   edge_capacity;
    int  *head;
    Edge *edges;
} EdgeListGraph;

EdgeListGraph *edge_list_graph_create(int vertex_count, int initial_edge_capacity);
void           edge_list_graph_add_edge(EdgeListGraph *graph, int from, int to, unsigned int weight);
void           edge_list_graph_free(EdgeListGraph *graph);


typedef struct {
    int           vertex_count;
    unsigned int *weights; 
} AdjacencyMatrixGraph;

AdjacencyMatrixGraph *adjacency_matrix_graph_create(int vertex_count);
void                  adjacency_matrix_graph_add_edge(AdjacencyMatrixGraph *graph, int from, int to, unsigned int weight);
void                  adjacency_matrix_graph_free(AdjacencyMatrixGraph *graph);
