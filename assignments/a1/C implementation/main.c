#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "hash_tables.h"

const int NUM_RUNS = 200; // how many times we average

struct profileProps {
    double loadFactor;
    int mapSize;
    int operationStyle;
    // 1 = insert only
    // 2 = insert + search
    // 3 = insert + search + delete
    // 4 = mixed
};

struct profileProps getProfileProps() {
    struct profileProps p;

    printf("Enter load factor: ");
    scanf("%lf", &p.loadFactor);

    printf("Enter map size: ");
    scanf("%d", &p.mapSize);

    printf("Select operation style:\n");
    printf("1. Insert Only\n");
    printf("2. Insert and Search all\n");
    printf("3. Insert, Search all, and Delete all\n");
    printf("4. Mixed Operations\n");
    printf("Choice (1-4): ");
    scanf("%d", &p.operationStyle);

    return p;
}

long long get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

int* generate_keys(int n) {
    int* arr = malloc(n * sizeof(int));
    srand(69); // lol

    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }

    return arr;
}

int compare_doubles(const void* a, const void* b) {
    double diff = *(double*)a - *(double*)b;
    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;
}

double get_median(double* times, int n) {
    qsort(times, n, sizeof(double), compare_doubles);

    if (n % 2 == 0)
        return (times[n/2 - 1] + times[n/2]) / 2.0;
    else
        return times[n/2];
}

void print_stats(double* times, int n, const char* label) {
    double median = get_median(times, n);

    double min = times[0];
    double max = times[0];

    for (int i = 1; i < n; i++) {
        if (times[i] < min) min = times[i];
        if (times[i] > max) max = times[i];
    }

    printf("%s -> median: %.2fms | min: %.2fms | max: %.2fms\n",
           label, median, min, max);
}


void run_insert_only(int* keys, int numKeys, int mapSize, int runs) {
    printf("\n=== Insert Only ===\n");

    double lp_times[runs];
    double ch_times[runs];

    for (int r = 0; r < runs; r++) {
        LinearProbingMap* lp = lp_create(mapSize);

        long long start = get_time_ns();
        for (int i = 0; i < numKeys; i++)
            lp_put(lp, keys[i], i);
        long long end = get_time_ns();

        lp_times[r] = (end - start) / 1000000.0;
        lp_destroy(lp);
    }

    for (int r = 0; r < runs; r++) {
        ChainingMap* ch = ch_create(mapSize);

        long long start = get_time_ns();
        for (int i = 0; i < numKeys; i++)
            ch_put(ch, keys[i], i);
        long long end = get_time_ns();

        ch_times[r] = (end - start) / 1000000.0;
        ch_destroy(ch);
    }

    print_stats(lp_times, runs, "Linear Probing");
    print_stats(ch_times, runs, "Chaining");
}


void run_insert_and_search(int* keys, int numKeys, int mapSize, int runs) {
    printf("\n=== Insert and Search All ===\n");

    double lp_times[runs];
    double ch_times[runs];

    for (int r = 0; r < runs; r++) {
        LinearProbingMap* lp = lp_create(mapSize);

        for (int i = 0; i < numKeys; i++)
            lp_put(lp, keys[i], i);

        long long start = get_time_ns();
        long long sum = 0;

        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += lp_get(lp, keys[i], &found);
        }

        long long end = get_time_ns();
        lp_times[r] = (end - start) / 1000000.0;

        if (sum < 0) printf("");
        lp_destroy(lp);
    }

    for (int r = 0; r < runs; r++) {
        ChainingMap* ch = ch_create(mapSize);

        for (int i = 0; i < numKeys; i++)
            ch_put(ch, keys[i], i);

        long long start = get_time_ns();
        long long sum = 0;

        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += ch_get(ch, keys[i], &found);
        }

        long long end = get_time_ns();
        ch_times[r] = (end - start) / 1000000.0;

        if (sum < 0) printf("");
        ch_destroy(ch);
    }

    print_stats(lp_times, runs, "Linear Probing");
    print_stats(ch_times, runs, "Chaining");
}


void run_insert_search_delete(int* keys, int numKeys, int mapSize, int runs) {
    printf("\n=== Insert, Search, Delete ===\n");

    double lp_insert[runs], lp_search[runs], lp_delete[runs];
    double ch_insert[runs], ch_search[runs], ch_delete[runs];

    for (int r = 0; r < runs; r++) {
        LinearProbingMap* lp = lp_create(mapSize);

        long long t1 = get_time_ns();
        for (int i = 0; i < numKeys; i++)
            lp_put(lp, keys[i], i);
        long long t2 = get_time_ns();

        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += lp_get(lp, keys[i], &found);
        }
        long long t3 = get_time_ns();

        for (int i = 0; i < numKeys; i++)
            lp_remove(lp, keys[i]);
        long long t4 = get_time_ns();

        lp_insert[r] = (t2 - t1) / 1000000.0;
        lp_search[r] = (t3 - t2) / 1000000.0;
        lp_delete[r] = (t4 - t3) / 1000000.0;

        if (sum < 0) printf("");
        lp_destroy(lp);
    }

    for (int r = 0; r < runs; r++) {
        ChainingMap* ch = ch_create(mapSize);

        long long t1 = get_time_ns();
        for (int i = 0; i < numKeys; i++)
            ch_put(ch, keys[i], i);
        long long t2 = get_time_ns();

        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += ch_get(ch, keys[i], &found);
        }
        long long t3 = get_time_ns();

        for (int i = 0; i < numKeys; i++)
            ch_remove(ch, keys[i]);
        long long t4 = get_time_ns();

        ch_insert[r] = (t2 - t1) / 1000000.0;
        ch_search[r] = (t3 - t2) / 1000000.0;
        ch_delete[r] = (t4 - t3) / 1000000.0;

        if (sum < 0) printf("");
        ch_destroy(ch);
    }

    printf("INSERT:\n");
    print_stats(lp_insert, runs, "Linear Probing");
    print_stats(ch_insert, runs, "Chaining");

    printf("\nSEARCH:\n");
    print_stats(lp_search, runs, "Linear Probing");
    print_stats(ch_search, runs, "Chaining");

    printf("\nDELETE:\n");
    print_stats(lp_delete, runs, "Linear Probing");
    print_stats(ch_delete, runs, "Chaining");
}


void run_mixed_operations(int* keys, int numKeys, int mapSize, int runs) {
    printf("\n=== Mixed (50 insert, 40 search, 10 delete) ===\n");

    double lp_times[runs];
    double ch_times[runs];

    for (int r = 0; r < runs; r++) {
        LinearProbingMap* lp = lp_create(mapSize);
        srand(42);

        long long start = get_time_ns();
        long long sum = 0;

        for (int i = 0; i < numKeys; i++) {
            double op = (double)rand() / RAND_MAX;

            if (op < 0.5)
                lp_put(lp, keys[i], i);
            else if (op < 0.9) {
                bool found;
                sum += lp_get(lp, keys[i], &found);
            }
            else
                lp_remove(lp, keys[i]);
        }

        long long end = get_time_ns();
        lp_times[r] = (end - start) / 1000000.0;

        if (sum < 0) printf("");
        lp_destroy(lp);
    }

    for (int r = 0; r < runs; r++) {
        ChainingMap* ch = ch_create(mapSize);
        srand(42);

        long long start = get_time_ns();
        long long sum = 0;

        for (int i = 0; i < numKeys; i++) {
            double op = (double)rand() / RAND_MAX;

            if (op < 0.5)
                ch_put(ch, keys[i], i);
            else if (op < 0.9) {
                bool found;
                sum += ch_get(ch, keys[i], &found);
            }
            else
                ch_remove(ch, keys[i]);
        }

        long long end = get_time_ns();
        ch_times[r] = (end - start) / 1000000.0;

        if (sum < 0) printf("");
        ch_destroy(ch);
    }

    print_stats(lp_times, runs, "Linear Probing");
    print_stats(ch_times, runs, "Chaining");
}

int main() {
    struct profileProps props;

    printf("Hashmap Profiler\n");
    printf("----------------\n\n");

    props = getProfileProps();

    printf("\nConfig:\n");
    printf("Map size: %d\n", props.mapSize);
    printf("Load factor: %.2f\n", props.loadFactor);
    printf("Runs: %d\n\n", NUM_RUNS);

    int numKeys = floor(props.mapSize * props.loadFactor);
    int* keys = generate_keys(numKeys);

    switch (props.operationStyle) {
        case 1:
            run_insert_only(keys, numKeys, props.mapSize, NUM_RUNS);
            break;
        case 2:
            run_insert_and_search(keys, numKeys, props.mapSize, NUM_RUNS);
            break;
        case 3:
            run_insert_search_delete(keys, numKeys, props.mapSize, NUM_RUNS);
            break;
        case 4:
            run_mixed_operations(keys, numKeys, props.mapSize, NUM_RUNS);
            break;
        default:
            printf("Invalid option\n");
    }

    free(keys);
    return 0;
}