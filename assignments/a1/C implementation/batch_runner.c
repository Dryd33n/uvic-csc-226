#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "hash_tables.h"

/*
   Batch profiler

   Sort of a copy of main.c with some modifications to run multiple experiments
   output is piped to a .csv in terminal
*/

const int NUM_RUNS = 10;   // run each test a few times and take median


long long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}


int* generate_keys(int n){
    int* arr = malloc(sizeof(int) * n);
    srand(69);

    for (int i = 0; i < n; i++){
        arr[i] = rand();
    }

    return arr;
}


int compare_doubles(const void* a, const void* b){
    double diff = *(double*)a - *(double*)b;

    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;
}


double get_median(double* times, int n){

    qsort(times, n, sizeof(double), compare_doubles);

    if (n % 2 == 0){
        return (times[n/2 - 1] + times[n/2]) / 2.0;
    }
    return times[n/2];
}



void run_mixed_operations(int* keys, int numKeys, int mapSize,
                          double* lp_median, double* ch_median){

    double lp_times[NUM_RUNS];
    double ch_times[NUM_RUNS];

    // Linear probing
    for (int r = 0; r < NUM_RUNS; r++){

        LinearProbingMap* lp = lp_create(mapSize);
        srand(42);

        long long start = get_time_ns();
        long long sum = 0;

        for (int i = 0; i < numKeys; i++){

            double op = (double)rand() / RAND_MAX;

            if (op < 0.5){
                lp_put(lp, keys[i], i);
            }
            else if (op < 0.9){
                bool found;
                sum += lp_get(lp, keys[i], &found);
            }
            else{
                lp_remove(lp, keys[i]);
            }
        }

        long long end = get_time_ns();
        lp_times[r] = (end - start) / 1000000.0;

        if (sum < 0) printf("");  // prevent optimizing away
        lp_destroy(lp);
    }


    // Separate chaining
    for (int r = 0; r < NUM_RUNS; r++){

        ChainingMap* ch = ch_create(mapSize);
        srand(42);

        long long start = get_time_ns();
        long long sum = 0;

        for (int i = 0; i < numKeys; i++){

            double op = (double)rand() / RAND_MAX;

            if (op < 0.5){
                ch_put(ch, keys[i], i);
            }
            else if (op < 0.9){
                bool found;
                sum += ch_get(ch, keys[i], &found);
            }
            else{
                ch_remove(ch, keys[i]);
            }
        }

        long long end = get_time_ns();
        ch_times[r] = (end - start) / 1000000.0;

        if (sum < 0) printf("");
        ch_destroy(ch);
    }

    *lp_median = get_median(lp_times, NUM_RUNS);
    *ch_median = get_median(ch_times, NUM_RUNS);
}



int main(){

    double loadFactors[] = {
        0.05,0.10,0.15,0.20,0.25,0.30,0.35,0.40,0.45,
        0.50,0.55,0.60,0.65,0.70,0.75,0.80,0.85,0.90,0.95
    };
    int numLF = 19;

    int mapSizes[] = {
        32,64,128,256,512,1024,2048,4096,
        16384,32768,65536,131072,262144,524288,
        1048576,2097152,4194304,8388608,16777216
    };
    int numSizes = 19;

    printf("MapSize,LoadFactor,NumKeys,LP_Median_ms,CH_Median_ms\n");

    int total = numLF * numSizes;
    int exp = 0;

    for (int s = 0; s < numSizes; s++){

        int mapSize = mapSizes[s];

        for (int l = 0; l < numLF; l++){

            double lf = loadFactors[l];
            int numKeys = (int)(mapSize * lf);

            exp++;

            int* keys = generate_keys(numKeys);

            double lp_median, ch_median;

            run_mixed_operations(keys, numKeys, mapSize,
                                 &lp_median, &ch_median);

            printf("%d,%.2f,%d,%.6f,%.6f\n",
                   mapSize, lf, numKeys, lp_median, ch_median);

            fflush(stdout);

            free(keys);
        }
    }

    return 0;
}