// loose implementation based off of
// https://github.com/dt2450/Dijkstras-Algorithm/blob/master/dijkstra_directed.c
// https://www.youtube.com/watch?v=ANZpc0zhc9Q
// https://github.com/TheAlgorithms/C/blob/master/greedy_approach/dijkstra.c
// https://github.com/TheAlgorithms/C/blob/master/data_structures/heap/min_heap.c
#include <stdlib.h>
#include "dijkstra.h"

typedef struct {
    int          *heap;
    int          *position;
    unsigned int *tentative_distance;
    int           size;
} MinHeap;

static MinHeap *min_heap_create(int vertex_count) {
    MinHeap *heap            = malloc(sizeof *heap);
    heap->heap               = malloc((size_t)(vertex_count + 1) * sizeof(int));
    heap->position           = calloc((size_t)(vertex_count + 1), sizeof(int));
    heap->tentative_distance = malloc((size_t)(vertex_count + 1) * sizeof(unsigned int));
    heap->size               = 0;
    return heap;
}

static void min_heap_free(MinHeap *heap) {
    free(heap->heap);
    free(heap->position);
    free(heap->tentative_distance);
    free(heap);
}

static void min_heap_swap(MinHeap *heap, int i, int j) {
    int vertex_i             = heap->heap[i];
    int vertex_j             = heap->heap[j];
    heap->heap[i]            = vertex_j;
    heap->heap[j]            = vertex_i;
    heap->position[vertex_i] = j;
    heap->position[vertex_j] = i;
}

static void min_heap_bubble_up(MinHeap *heap, int index) {
    while (index > 1 &&
           heap->tentative_distance[heap->heap[index]] <
           heap->tentative_distance[heap->heap[index >> 1]]) {
        min_heap_swap(heap, index, index >> 1);
        index >>= 1;
    }
}

static void min_heap_bubble_down(MinHeap *heap, int index) {
    for (;;) {
        int smallest = index;
        int left     = index << 1;
        int right    = left | 1;
        if (left  <= heap->size &&
            heap->tentative_distance[heap->heap[left]] <
            heap->tentative_distance[heap->heap[smallest]])
            smallest = left;
        if (right <= heap->size &&
            heap->tentative_distance[heap->heap[right]] <
            heap->tentative_distance[heap->heap[smallest]])
            smallest = right;
        if (smallest == index) break;
        min_heap_swap(heap, index, smallest);
        index = smallest;
    }
}

static void min_heap_push(MinHeap *heap, int vertex, unsigned int distance) {
    heap->tentative_distance[vertex] = distance;
    heap->heap[++heap->size]         = vertex;
    heap->position[vertex]           = heap->size;
    min_heap_bubble_up(heap, heap->size);
}

static void min_heap_decrease_key(MinHeap *heap, int vertex, unsigned int new_distance) {
    heap->tentative_distance[vertex] = new_distance;
    min_heap_bubble_up(heap, heap->position[vertex]);
}

static int min_heap_pop(MinHeap *heap) {
    int minimum_vertex             = heap->heap[1];
    heap->position[minimum_vertex] = 0;
    heap->heap[1]                  = heap->heap[heap->size--];
    if (heap->size) {
        heap->position[heap->heap[1]] = 1;
        min_heap_bubble_down(heap, 1);
    }
    return minimum_vertex;
}


// dijkstras algorithm
unsigned int *dijkstra(const EdgeListGraph *graph, int source_vertex) {
    int           vertex_count = graph->vertex_count;
    unsigned int *distance     = malloc((size_t)vertex_count * sizeof(unsigned int)); // distance array

    // initialization: give all vertices infinite weight then correct source vertex to 0
    for (int vertex = 0; vertex < vertex_count; vertex++)
        distance[vertex] = INFINITE_WEIGHT;
    distance[source_vertex] = 0;

    // min heap setup
    MinHeap *heap = min_heap_create(vertex_count);
    min_heap_push(heap, source_vertex, 0);

    // while there are still vertices to process
    while (heap->size) {
        // get the vertex with the smallest tentative distance
        int current_vertex = min_heap_pop(heap);

        // for each edge from the current vertex, relax the edge
        for (int edge_index = graph->head[current_vertex];
             edge_index != -1;
             edge_index = graph->edges[edge_index].next_edge_index) {

            const Edge   *edge         = &graph->edges[edge_index];
            unsigned int  new_distance = distance[current_vertex] + edge->weight;

            // if a shorter path to the destination vertex is found, update the distance and heap
            if (new_distance < distance[edge->destination]) {
                distance[edge->destination] = new_distance;
                // update heap: decrease key if already present, otherwise push
                if (heap->position[edge->destination])
                    min_heap_decrease_key(heap, edge->destination, new_distance);
                else
                    min_heap_push(heap, edge->destination, new_distance);
            }
        }
    }

    // termination
    min_heap_free(heap);
    return distance;
}
