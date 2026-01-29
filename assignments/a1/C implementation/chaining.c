#include "hash_tables.h"
#include <stdlib.h>

// Hash function - same as linear probing for fair comparison
static size_t hash(int key, size_t capacity) {
    unsigned int uk = (unsigned int)key;
    return uk % capacity;
}

// Create a new chaining hash map
ChainingMap* ch_create(size_t capacity) {
    ChainingMap* map = malloc(sizeof(ChainingMap));
    
    map->capacity = capacity;
    map->size = 0;
    
    // Allocate array of node pointers, initialize all to NULL
    map->table = calloc(capacity, sizeof(Node*));
    
    return map;
}

// Insert or update a key-value pair
void ch_put(ChainingMap* map, int key, int value) {
    size_t i = hash(key, map->capacity);
    
    // Search the chain at this index
    Node* current = map->table[i];
    while (current != NULL) {
        if (current->key == key) {
            // Key already exists - update value
            current->value = value;
            return;
        }
        current = current->next;
    }
    
    // Key not found - create new node and add at head of list
    Node* new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = map->table[i];  // Point to old head
    map->table[i] = new_node;        // New node becomes head
    map->size++;
}

// Get value for a key
int ch_get(ChainingMap* map, int key, bool* found) {
    size_t i = hash(key, map->capacity);
    
    // Walk the chain looking for the key
    Node* current = map->table[i];
    while (current != NULL) {
        if (current->key == key) {
            *found = true;
            return current->value;
        }
        current = current->next;
    }
    
    // Key not found
    *found = false;
    return 0;
}

// Remove a key-value pair
bool ch_remove(ChainingMap* map, int key) {
    size_t i = hash(key, map->capacity);
    
    Node* current = map->table[i];
    Node* prev = NULL;
    
    // Walk the chain
    while (current != NULL) {
        if (current->key == key) {
            // Found it - remove from chain
            if (prev == NULL) {
                // Removing head of list
                map->table[i] = current->next;
            } else {
                // Removing middle or end of list
                prev->next = current->next;
            }
            
            free(current);
            map->size--;
            return true;
        }
        
        prev = current;
        current = current->next;
    }
    
    // Key not found
    return false;
}

// Get number of elements
size_t ch_size(ChainingMap* map) {
    return map->size;
}

// Get load factor
double ch_load_factor(ChainingMap* map) {
    return (double)map->size / (double)map->capacity;
}

// Free all memory
void ch_destroy(ChainingMap* map) {
    // Free all nodes in all chains
    for (size_t i = 0; i < map->capacity; i++) {
        Node* current = map->table[i];
        while (current != NULL) {
            Node* next = current->next;
            free(current);
            current = next;
        }
    }
    
    // Free the table array
    free(map->table);
    
    // Free the map struct itself
    free(map);
}