#include "hash_tables.h"
#include <stdlib.h>

/* HASH TABLE WITH OPEN ADDRESSING AND LINEAR PROBING IMPLEMENTATION
 * * * Credits: * This implementation is adapted from the implementation
 * provided from GeeksforGeeks *
 * https://www.geeksforgeeks.org/dsa/program-to-implement-hash-table-using-open-addressing/ */

#define EMPTY -1
#define TOMBSTONE -2


static size_t hash(int key, size_t capacity) {
    unsigned int k = (unsigned int) key;
    return k % capacity;
}


LinearProbingMap* lp_create(size_t capacity) {

    LinearProbingMap* m = malloc(sizeof(LinearProbingMap));

    m->capacity = capacity;
    m->size = 0;

    m->keys = malloc(sizeof(int) * capacity);
    m->values = malloc(sizeof(int) * capacity);

    // mark everything empty
    for (size_t i = 0; i < capacity; i++){
        m->keys[i] = EMPTY;
    }

    return m;
}



void lp_put(LinearProbingMap* map, int key, int value){

    size_t idx = hash(key, map->capacity);

    // probe until we find spot
    while (map->keys[idx] != EMPTY && map->keys[idx] != TOMBSTONE){

        if (map->keys[idx] == key){
            // overwrite existing value
            map->values[idx] = value;
            return;
        }

        idx = (idx + 1) % map->capacity;
    }

    map->keys[idx] = key;
    map->values[idx] = value;
    map->size++;
}



int lp_get(const LinearProbingMap* map, int key, bool* found){

    size_t idx = hash(key, map->capacity);

    while (map->keys[idx] != EMPTY){

        if (map->keys[idx] == key){
            *found = true;
            return map->values[idx];
        }

        idx = (idx + 1) % map->capacity;
    }

    *found = false;
    return 0;
}



bool lp_remove(LinearProbingMap* map, int key){

    size_t idx = hash(key, map->capacity);

    while (map->keys[idx] != EMPTY){

        if (map->keys[idx] == key){
            map->keys[idx] = TOMBSTONE;   // mark deleted
            map->values[idx] = 0;
            map->size--;
            return true;
        }

        idx = (idx + 1) % map->capacity;
    }

    return false;
}



void lp_destroy(LinearProbingMap* map){

    free(map->keys);
    free(map->values);
    free(map);
}