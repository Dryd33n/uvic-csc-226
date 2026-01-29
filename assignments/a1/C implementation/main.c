#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "hash_tables.h"


const int NUM_RUNS = 200; // Number of runs for averaging

struct profileProps {
    double loadFactor;
    int mapSize;
    int operationStyle; // 1. Insert Only
                        // 2. Insert and Search all
                        // 3. Insert, Search all, and Delete all
                        // 4. Mixed Operations
};

// Get profile props from CLI
struct profileProps getProfileProps() {
    struct profileProps props;

    // Get load factor from user
    printf("Enter load factor: ");
    scanf("%lf", &props.loadFactor);

    // Get map size from user
    printf("Enter map size: ");
    scanf("%d", &props.mapSize);


    // Get operation style from user
    printf("Select operation style:\n");
    printf("1. Insert Only\n");
    printf("2. Insert and Search all\n");
    printf("3. Insert, Search all, and Delete all\n");
    printf("4. Mixed Operations\n");
    printf("Enter choice (1-4): ");
    scanf("%d", &props.operationStyle);

    return props;
}

// Get time in nanoseconds
// Credits: https://stackoverflow.com/questions/62910438/monotonic-clock-in-c
long long get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

// Generate random keys
int* generate_keys(int n) {
    int* keys = malloc(n * sizeof(int));
    srand(69); // Nice
    for (int i = 0; i < n; i++) {
        keys[i] = rand();
    }
    return keys;
}

int compare_doubles(const void* a, const void* b) {
    const double diff = *(double*)a - *(double*)b;
    return (diff > 0) - (diff < 0);
}

// Credits:
// https://www.geeksforgeeks.org/c/find-median-of-numbers-in-an-array-in-c/
double get_median(double* times, int n) {
    qsort(times, n, sizeof(double), compare_doubles);
    if (n % 2 == 0) {
        return (times[n/2 - 1] + times[n/2]) / 2.0;
    } else {
        return times[n/2];
    }
}

// Print statistics
void print_stats(double* times, const int n, const char* name) {
    const double median = get_median(times, n);

    double min = times[0];
    double max = times[0];
    for (int i = 1; i < n; i++) {
        if (times[i] < min) min = times[i];
        if (times[i] > max) max = times[i];
    }

    printf("%s: median=%.2fms, min=%.2fms, max=%.2fms\n", name, median, min, max);
}




// Operation Style 1: Insert Only
void run_insert_only(const int* keys, const int numKeys, const int mapSize, const int num_runs) {
    printf("\n=== Insert Only ===\n");

    double lp_times[num_runs];
    double ch_times[num_runs];

    // Linear Probing
    for (int run = 0; run < num_runs; run++) {
        LinearProbingMap* lp = lp_create(mapSize);
        const long long start = get_time_ns();
        for (int i = 0; i < numKeys; i++) {
            lp_put(lp, keys[i], i);
        }
        const long long end = get_time_ns();
        lp_times[run] = (end - start) / 1000000.0;
        lp_destroy(lp);
    }

    // Chaining
    for (int run = 0; run < num_runs; run++) {
        ChainingMap* ch = ch_create(mapSize);
        const long long start = get_time_ns();
        for (int i = 0; i < numKeys; i++) {
            ch_put(ch, keys[i], i);
        }
        const long long end = get_time_ns();
        ch_times[run] = (end - start) / 1000000.0;
        ch_destroy(ch);
    }

    print_stats(lp_times, num_runs, "Linear Probing");
    print_stats(ch_times, num_runs, "Chaining");
}


// Operation Style 2: Insert and Search All
void run_insert_and_search(const int* keys, const int numKeys, const int mapSize, const int num_runs) {
    printf("\n=== Insert and Search All ===\n");

    double lp_times[num_runs];
    double ch_times[num_runs];

    // Linear Probing
    for (int run = 0; run < num_runs; run++) {
        LinearProbingMap* lp = lp_create(mapSize);
        for (int i = 0; i < numKeys; i++) {
            lp_put(lp, keys[i], i);
        }

        long long start = get_time_ns();
        long long sum = 0;  // Prevent optimization
        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += lp_get(lp, keys[i], &found);
        }
        long long end = get_time_ns();
        lp_times[run] = (end - start) / 1000000.0;

        if (sum < 0) printf("");  // Use sum
        lp_destroy(lp);
    }

    // Chaining
    for (int run = 0; run < num_runs; run++) {
        ChainingMap* ch = ch_create(mapSize);
        for (int i = 0; i < numKeys; i++) {
            ch_put(ch, keys[i], i);
        }

        long long start = get_time_ns();
        long long sum = 0;  // Prevent optimization
        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += ch_get(ch, keys[i], &found);
        }
        long long end = get_time_ns();
        ch_times[run] = (end - start) / 1000000.0;

        if (sum < 0) printf("");  // Use sum
        ch_destroy(ch);
    }

    print_stats(lp_times, num_runs, "Linear Probing");
    print_stats(ch_times, num_runs, "Chaining");
}


// Operation Style 3: Insert, Search All, and Delete All
void run_insert_search_delete(const int* keys, const int numKeys, const int mapSize, const int num_runs) {
    printf("\n=== Insert, Search All, and Delete All ===\n");

    double lp_insert_times[num_runs];
    double lp_search_times[num_runs];
    double lp_delete_times[num_runs];
    double ch_insert_times[num_runs];
    double ch_search_times[num_runs];
    double ch_delete_times[num_runs];

    // Linear Probing
    for (int run = 0; run < num_runs; run++) {
        LinearProbingMap* lp = lp_create(mapSize);

        // Insert
        long long t1 = get_time_ns();
        for (int i = 0; i < numKeys; i++) {
            lp_put(lp, keys[i], i);
        }
        long long t2 = get_time_ns();

        // Search
        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += lp_get(lp, keys[i], &found);
        }
        long long t3 = get_time_ns();

        // Delete
        for (int i = 0; i < numKeys; i++) {
            lp_remove(lp, keys[i]);
        }
        long long t4 = get_time_ns();

        lp_insert_times[run] = (t2 - t1) / 1000000.0;
        lp_search_times[run] = (t3 - t2) / 1000000.0;
        lp_delete_times[run] = (t4 - t3) / 1000000.0;

        if (sum < 0) printf("");
        lp_destroy(lp);
    }

    // Chaining
    for (int run = 0; run < num_runs; run++) {
        ChainingMap* ch = ch_create(mapSize);

        // Insert
        long long t1 = get_time_ns();
        for (int i = 0; i < numKeys; i++) {
            ch_put(ch, keys[i], i);
        }
        long long t2 = get_time_ns();

        // Search
        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            bool found;
            sum += ch_get(ch, keys[i], &found);
        }
        long long t3 = get_time_ns();

        // Delete
        for (int i = 0; i < numKeys; i++) {
            ch_remove(ch, keys[i]);
        }
        long long t4 = get_time_ns();

        ch_insert_times[run] = (t2 - t1) / 1000000.0;
        ch_search_times[run] = (t3 - t2) / 1000000.0;
        ch_delete_times[run] = (t4 - t3) / 1000000.0;

        if (sum < 0) printf("");
        ch_destroy(ch);
    }

    printf("INSERT:\n");
    print_stats(lp_insert_times, num_runs, "  Linear Probing");
    print_stats(ch_insert_times, num_runs, "  Chaining");

    printf("\nSEARCH:\n");
    print_stats(lp_search_times, num_runs, "  Linear Probing");
    print_stats(ch_search_times, num_runs, "  Chaining");

    printf("\nDELETE:\n");
    print_stats(lp_delete_times, num_runs, "  Linear Probing");
    print_stats(ch_delete_times, num_runs, "  Chaining");
}


// Operation Style 4: Mixed Operations
void run_mixed_operations(const int* keys, const int numKeys, const int mapSize, const int num_runs) {
    printf("\n=== Mixed Operations (50%% insert, 40%% search, 10%% delete) ===\n");

    double lp_times[num_runs];
    double ch_times[num_runs];

    // Linear Probing
    for (int run = 0; run < num_runs; run++) {
        LinearProbingMap* lp = lp_create(mapSize);
        srand(42);  // Reset seed for reproducibility

        long long start = get_time_ns();
        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            double op = (double)rand() / RAND_MAX;
            if (op < 0.5) {
                // 50% insert
                lp_put(lp, keys[i], i);
            } else if (op < 0.9) {
                // 40% search
                bool found;
                sum += lp_get(lp, keys[i], &found);
            } else {
                // 10% delete
                lp_remove(lp, keys[i]);
            }
        }
        long long end = get_time_ns();

        lp_times[run] = (end - start) / 1000000.0;
        if (sum < 0) printf("");
        lp_destroy(lp);
    }

    // Chaining
    for (int run = 0; run < num_runs; run++) {
        ChainingMap* ch = ch_create(mapSize);
        srand(42);  // Reset seed for reproducibility

        long long start = get_time_ns();
        long long sum = 0;
        for (int i = 0; i < numKeys; i++) {
            double op = (double)rand() / RAND_MAX;
            if (op < 0.5) {
                // 50% insert
                ch_put(ch, keys[i], i);
            } else if (op < 0.9) {
                // 40% search
                bool found;
                sum += ch_get(ch, keys[i], &found);
            } else {
                // 10% delete
                ch_remove(ch, keys[i]);
            }
        }
        long long end = get_time_ns();

        ch_times[run] = (end - start) / 1000000.0;
        if (sum < 0) printf("");
        ch_destroy(ch);
    }

    print_stats(lp_times, num_runs, "Linear Probing");
    print_stats(ch_times, num_runs, "Chaining");
}


int main(void) {
    bool cliPropsProvided = true;
    struct profileProps props;

    printf("Hashmap Profiler (Separate Chaining VS. Linear Probing)\n");
    printf("========================================================\n\n");

    if (cliPropsProvided) {
        props = getProfileProps();
    } else {
        // use alternate running approach
        // runFunc()
        return 0;
    }

    printf("\nConfiguration:\n");
    printf("  Map Size: %d\n", props.mapSize);
    printf("  Load Factor: %.2f\n", props.loadFactor);
    printf("  Runs: %d\n\n", NUM_RUNS);

    // Generate keys
    const int numKeys = floor(props.mapSize * props.loadFactor);
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
            printf("Invalid operation style.\n");
    }

    free(keys);
    return 0;
}