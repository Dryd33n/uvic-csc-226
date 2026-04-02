#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include "graph.h"
#include "graph_generator.h"
#include "dijkstra.h"
#include "floyd_warshall_roy.h"
#include "experiment.h"

#define TRIALS             10
#define DENSITY_COUNT      4
#define VERTEX_COUNT_COUNT 20
#define K_FRACTION_COUNT   10

static double densities[]     = { 0.25, 0.5, 0.75, 1.0 };
static int    vertex_counts[] = { 25, 50, 75, 100, 125, 150, 175, 200, 225, 250,
                                   275, 300, 325, 350, 375, 400, 425, 450, 475, 500 };
static double k_fractions[]   = { 0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0 };

static int compare_doubles(const void *a, const void *b) {
    double diff = *(double *)a - *(double *)b;
    if (diff < 0) return -1;
    if (diff > 0) return  1;
    return 0;
}

// returns the median of an array of doubles, array must be sorted
static double median(double *sorted_values, int count) {
    if (count % 2 == 0)
        return (sorted_values[count / 2 - 1] + sorted_values[count / 2]) / 2.0;
    return sorted_values[count / 2];
}

// returns the standard deviation of an array of doubles
static double stddev(double *values, int count) {
    double sum = 0.0;
    for (int i = 0; i < count; i++)
        sum += values[i];
    double mean = sum / count;

    double variance = 0.0;
    for (int i = 0; i < count; i++)
        variance += (values[i] - mean) * (values[i] - mean);
    return sqrt(variance / count);
}

int main(void) {
    srand(time(NULL));

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    FILE *csv = fopen("results.csv", "w");
    if (!csv) {
        fprintf(stderr, "error: could not open results.csv for writing\n");
        return 1;
    }

    // print csv header
    fprintf(csv, "density,vertex_count,k,dijkstra_median_ms,fwr_median_ms,dijkstra_stddev_ms,fwr_stddev_ms\n");

    // used for keeping track of how many experiements are being run in total
    int total_combinations = DENSITY_COUNT * VERTEX_COUNT_COUNT * K_FRACTION_COUNT;
    int combinations_completed = 0;

    // for all densities
    for (int density_index = 0; density_index < DENSITY_COUNT; density_index++) {
        double density = densities[density_index];

        // for all vertice count for the given density
        for (int vc_index = 0; vc_index < VERTEX_COUNT_COUNT; vc_index++) {
            int vertex_count = vertex_counts[vc_index];

            //for all values of k given density and vertex count
            for (int k_index = 0; k_index < K_FRACTION_COUNT; k_index++) {
                int k = (int)(k_fractions[k_index] * vertex_count);
                if (k < 1) k = 1;

                combinations_completed++;

                // print to terminal for keeping track of wtf is going on
                printf("combination %d / %d  |  density=%.2f  vertex_count=%d  k=%d\n",
                       combinations_completed, total_combinations,
                       density, vertex_count, k);

                // arrays of doubles to hold times
                double dijkstra_times[TRIALS];
                double fwr_times[TRIALS];

                // run multiple trials for this combination of parameters
                for (int trial = 0; trial < TRIALS; trial++) {
                    // generate a random graph for each trial
                    EdgeListGraph        *edge_list_graph        = NULL;
                    AdjacencyMatrixGraph *adjacency_matrix_graph = NULL;
                    generate_random_graph(vertex_count, density,
                                          &edge_list_graph, &adjacency_matrix_graph);

                    // generate random source vertices for this trial
                    int *sources = generate_source_vertices(vertex_count, k);

                    // time dijkstras
                    LARGE_INTEGER dijkstra_start, dijkstra_end;
                    QueryPerformanceCounter(&dijkstra_start);
                    unsigned int **dijkstra_distances = dijkstra_k_sources(edge_list_graph, sources, k);
                    QueryPerformanceCounter(&dijkstra_end);

                    //time fwr
                    LARGE_INTEGER fwr_start, fwr_end;
                    QueryPerformanceCounter(&fwr_start);
                    unsigned int *fwr_distances = floyd_warshall_roy(adjacency_matrix_graph);
                    QueryPerformanceCounter(&fwr_end);

                    // calculate times in milliseconds and store in arrays
                    dijkstra_times[trial] = (double)(dijkstra_end.QuadPart - dijkstra_start.QuadPart)
                                            / frequency.QuadPart * 1000.0;
                    fwr_times[trial]      = (double)(fwr_end.QuadPart - fwr_start.QuadPart)
                                            / frequency.QuadPart * 1000.0;

                    for (int i = 0; i < k; i++) free(dijkstra_distances[i]);

                    // free allocated memory
                    free(dijkstra_distances);
                    free(fwr_distances);
                    free(sources);
                    edge_list_graph_free(edge_list_graph);
                    adjacency_matrix_graph_free(adjacency_matrix_graph);
                }

                // sort the times in order to find median
                qsort(dijkstra_times, TRIALS, sizeof(double), compare_doubles);
                qsort(fwr_times,      TRIALS, sizeof(double), compare_doubles);

                // write to csv file
                fprintf(csv, "%.2f,%d,%d,%.4f,%.4f,%.4f,%.4f\n",
                        density, vertex_count, k,
                        median(dijkstra_times, TRIALS),
                        median(fwr_times,      TRIALS),
                        stddev(dijkstra_times, TRIALS),
                        stddev(fwr_times,      TRIALS));

                // saves it to csv after every combination so if the program crashes we still have some results
                fflush(csv);
            }
        }
    }

    // close csv file
    fclose(csv);
    return 0;
}