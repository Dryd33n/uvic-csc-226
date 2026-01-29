#ifndef HASH_TABLES_H
#define HASH_TABLES_H

#include <stddef.h>
#include <stdbool.h>

// ==================== LINEAR PROBING ====================

typedef struct {
    int* keys;
    int* values;
    size_t capacity;
    size_t size;
} LinearProbingMap;

// Create a linear probing hash map with specified capacity
LinearProbingMap* lp_create(size_t capacity);

// Insert or update a key-value pair
void lp_put(LinearProbingMap* map, int key, int value);

// Get value for a key. Sets 'found' to true if key exists, false otherwise
int lp_get(const LinearProbingMap* map, int key, bool* found);

// Remove a key-value pair. Returns true if key was found and removed
bool lp_remove(LinearProbingMap* map, int key);

// Get number of elements in the map
size_t lp_size(const LinearProbingMap* map);

// Get current load factor (size / capacity)
double lp_load_factor(const LinearProbingMap* map);

// Free all memory used by the map
void lp_destroy(LinearProbingMap* map);


// ==================== CHAINING ====================

// Node in a linked list
typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node** table;      // Array of pointers to linked lists
    size_t capacity;
    size_t size;
} ChainingMap;

// Create a chaining hash map with specified capacity
ChainingMap* ch_create(size_t capacity);

// Insert or update a key-value pair
void ch_put(ChainingMap* map, int key, int value);

// Get value for a key. Sets 'found' to true if key exists, false otherwise
int ch_get(ChainingMap* map, int key, bool* found);

// Remove a key-value pair. Returns true if key was found and removed
bool ch_remove(ChainingMap* map, int key);

// Get number of elements in the map
size_t ch_size(ChainingMap* map);

// Get current load factor (size / capacity)
double ch_load_factor(ChainingMap* map);

// Free all memory used by the map
void ch_destroy(ChainingMap* map);

#endif // HASH_TABLES_H