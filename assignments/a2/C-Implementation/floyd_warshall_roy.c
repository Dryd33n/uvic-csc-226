// implementation loosely based off of:
// https://github.com/TheAlgorithms/C/blob/master/data_structures/graphs/floyd_warshall.c
// https://www.youtube.com/watch?v=ANZpc0zhc9Q


#include <stdlib.h>
#include "floyd_warshall_roy.h"

unsigned int *floyd_warshall_roy(const AdjacencyMatrixGraph *graph) {
    int           vertex_count = graph->vertex_count;
    size_t        matrix_size  = (size_t)vertex_count * (size_t)vertex_count;
    unsigned int *distance     = malloc(matrix_size * sizeof(unsigned int)); // distance matrix

    // initialization: copy the weights from the graph to the distance matrix
    for (size_t index = 0; index < matrix_size; index++)
        distance[index] = graph->weights[index];

    // for each intermediate vertex
    for (int intermediate = 0; intermediate < vertex_count; intermediate++) {

        // for each source vertex
        for (int source = 0; source < vertex_count; source++) {
            // compute distance from source to intermediate
            unsigned int source_to_intermediate = distance[source * vertex_count + intermediate];
            // skip if there is no path from source to intermediate
            if (source_to_intermediate == INFINITE_WEIGHT) continue;

            // for each destination vertex
            for (int destination = 0; destination < vertex_count; destination++) {
                // compute distance from intermediate to destination
                unsigned int intermediate_to_destination = distance[intermediate * vertex_count + destination];
                // skip if there is no path from intermediate to destination
                if (intermediate_to_destination == INFINITE_WEIGHT) continue;

                // compute the total distance from source to destination through intermediate
                unsigned int path_through_intermediate = source_to_intermediate;

                // if overflow occurs due to inf + finite or inf + inf, set infinite
                if (path_through_intermediate < source_to_intermediate)
                    path_through_intermediate = INFINITE_WEIGHT;

                // update distance if a shorter path is found
                if (path_through_intermediate < distance[source * vertex_count + destination])
                    distance[source * vertex_count + destination] = path_through_intermediate;
            }
        }
    }

    return distance;
}
