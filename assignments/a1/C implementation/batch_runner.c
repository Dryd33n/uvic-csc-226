#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "hash_tables.h"

const int NUM_RUNS = 10;  // Reduced for grid experiments

long long get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

int* generate_keys(int n) {
    int* keys = malloc(n * sizeof(int));
    srand(42);
    for (int i = 0; i < n; i++) {
        keys[i] = rand();
    }
    return keys;
}

int compare_doubles(const void* a, const void* b) {
    const double diff = *(double*)a - *(double*)b;
    return (diff > 0) - (diff < 0);
}

double get_median(double* times, int n) {
    qsort(times, n, sizeof(double), compare_doubles);
    if (n % 2 == 0) {
        return (times[n/2 - 1] + times[n/2]) / 2.0;
    } else {
        return times[n/2];
    }
}

void benchmark_mixed(const int* keys, const int numKeys, const int mapSize,
                    double* lp_median, double* ch_median) {
    double lp_times[NUM_RUNS];
    double ch_times[NUM_RUNS];
    
    // Linear Probing
    for (int run = 0; run < NUM_RUNS; run++) {
        LinearProbingMap* lp = lp_create(mapSize);
        srand(42);
        
        long long start = get_time_ns();
        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            double op = (double)rand() / RAND_MAX;
            if (op < 0.5) {
                lp_put(lp, keys[i], i);
            } else if (op < 0.9) {
                bool found;
                sum += lp_get(lp, keys[i], &found);
            } else {
                lp_remove(lp, keys[i]);
            }
        }
        long long end = get_time_ns();
        lp_times[run] = (end - start) / 1000000.0;
        
        if (sum < 0) printf("");
        lp_destroy(lp);
    }
    
    // Chaining
    for (int run = 0; run < NUM_RUNS; run++) {
        ChainingMap* ch = ch_create(mapSize);
        srand(42);
        
        long long start = get_time_ns();
        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            double op = (double)rand() / RAND_MAX;
            if (op < 0.5) {
                ch_put(ch, keys[i], i);
            } else if (op < 0.9) {
                bool found;
                sum += ch_get(ch, keys[i], &found);
            } else {
                ch_remove(ch, keys[i]);
            }
        }
        long long end = get_time_ns();
        ch_times[run] = (end - start) / 1000000.0;
        
        if (sum < 0) printf("");
        ch_destroy(ch);
    }
    
    *lp_median = get_median(lp_times, NUM_RUNS);
    *ch_median = get_median(ch_times, NUM_RUNS);
}

int main(void) {
    printf("=== Hash Map Performance - Mixed Operations (50%% insert, 40%% search, 10%% delete) ===\n\n");
    
    // Define grid
    const double loadFactors[] = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95};
    const int numLF = 19;
    
    const int mapSizes[] = {32, 64, 128, 256, 512, 1024, 2048, 4096, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216};
    const int numSizes = 19;
    
    printf("Grid Configuration:\n");
    printf("  Load Factors: %d points (%.2f to %.2f)\n", numLF, loadFactors[0], loadFactors[numLF-1]);
    printf("  Map Sizes: %d points (%d to %d)\n", numSizes, mapSizes[0], mapSizes[numSizes-1]);
    printf("  Total experiments: %d\n", numLF * numSizes);
    printf("  Runs per experiment: %d\n\n", NUM_RUNS);
    
    // Print CSV header
    printf("MapSize,LoadFactor,NumKeys,LP_Median_ms,CH_Median_ms\n");
    
    int experiment = 0;
    int total = numLF * numSizes;
    
    // Run grid experiments
    for (int s = 0; s < numSizes; s++) {
        int mapSize = mapSizes[s];
        
        for (int l = 0; l < numLF; l++) {
            double lf = loadFactors[l];
            int numKeys = (int)(mapSize * lf);
            
            experiment++;
            fprintf(stderr, "[%d/%d] MapSize=%d, LoadFactor=%.2f, NumKeys=%d...\n", 
                    experiment, total, mapSize, lf, numKeys);
            
            int* keys = generate_keys(numKeys);
            
            double lp_median, ch_median;
            benchmark_mixed(keys, numKeys, mapSize, &lp_median, &ch_median);
            
            printf("%d,%.2f,%d,%.6f,%.6f\n", mapSize, lf, numKeys, lp_median, ch_median);
            fflush(stdout);
            
            free(keys);
        }
    }
    
    fprintf(stderr, "\n=== Complete! Data saved to CSV ===\n");
    
    return 0;
}