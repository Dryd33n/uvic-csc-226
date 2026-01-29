#include "hash_tables.h"
#include <stdlib.h>

/* HASH TABLE WITH OPEN ADDRESSING AND LINEAR PROBING IMPLEMENTATION
 *
 * Credits:
 * This implementation is adapted from the implementation provided from GeeksforGeeks
 * https://www.geeksforgeeks.org/dsa/program-to-implement-hash-table-using-open-addressing/
 */




// Special markers for array slots
#define EMPTY -1
#define TOMBSTONE -2





static size_t hash(int key, size_t capacity) {
    unsigned int uk = (unsigned int)key;
    return uk % capacity;
}





LinearProbingMap* lp_create(size_t capacity) {
    LinearProbingMap* map = malloc(sizeof(LinearProbingMap));
    
    map->capacity = capacity;
    map->size = 0;
    map->keys = malloc(capacity * sizeof(int));
    map->values = malloc(capacity * sizeof(int));
    
    // Initialize all slots as empty
    for (size_t i = 0; i < capacity; i++) {
        map->keys[i] = EMPTY;
    }
    
    return map;
}





void lp_put(LinearProbingMap* map, int key, int value) {
    // Get starting position
    size_t i = hash(key, map->capacity);
    
    // Linear probe until we find an empty/tombstone slot or the key
    while (map->keys[i] != EMPTY && map->keys[i] != TOMBSTONE) {
        if (map->keys[i] == key) {
            // Key already exists - update value
            map->values[i] = value;
            return;
        }
        // Move to next slot (wrap around if needed)
        i = (i + 1) % map->capacity;
    }
    
    // Found empty slot or tombstone - insert new entry
    map->keys[i] = key;
    map->values[i] = value;
    map->size++;
}

// Get value for a key
int lp_get(const LinearProbingMap* map, const int key, bool* found) {
    size_t i = hash(key, map->capacity);
    
    // Keep probing until we find the key or hit an empty slot
    while (map->keys[i] != EMPTY) {
        if (map->keys[i] == key) {
            // Found it!
            *found = true;
            return map->values[i];
        }
        i = (i + 1) % map->capacity;
    }
    
    // Didn't find it
    *found = false;
    return 0;
}

// Remove a key-value pair
bool lp_remove(LinearProbingMap* map, const int key) {
    size_t i = hash(key, map->capacity);
    
    // Search for the key
    while (map->keys[i] != EMPTY) {
        if (map->keys[i] == key) {
            // Found it - mark as tombstone
            map->keys[i] = TOMBSTONE;
            map->values[i] = 0; // Optional cleanup
            map->size--;
            return true;
        }
        i = (i + 1) % map->capacity;
    }
    
    // Key not found
    return false;
}

// Free all memory
void lp_destroy(LinearProbingMap* map) {
    free(map->keys);
    free(map->values);
    free(map);
}