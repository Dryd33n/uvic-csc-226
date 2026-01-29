#include "hash_tables.h"
#include <stdlib.h>

/* HASH TABLE WITH SEPARATE CHAINING IMPLEMENTATION
 *
 * Credits:
 * This implementation is adapted from the implementation provided from GeeksforGeeks
 * https://www.geeksforgeeks.org/dsa/implementation-of-hash-table-in-c-using-separate-chaining/
 */




// Standard hashing function, this is the same as in linear_probing.c
static size_t hash(const int key, const size_t capacity) {
    const unsigned int uk = (unsigned int)key; //makes any negative key positive
    return uk % capacity;
}





// create hash table using chaining
ChainingMap* ch_create(const size_t capacity) {
    ChainingMap* map = malloc(sizeof(ChainingMap)); //allocate space for map struct
    
    map->capacity = capacity; //set capacity
    map->size = 0; //intialize number of elements to 0
    

    map->table = calloc(capacity, sizeof(Node*));  // allocated space for nodes in array
    
    return map;
}





void ch_put(ChainingMap* map, const int key, const int value) {
    const size_t insertionIndex = hash(key, map->capacity); //get index from hash function

    // look through linked list at that index
    Node* current = map->table[insertionIndex];
    while (current != NULL) { //keep looking as long as node isn't null
        if (current->key == key) { //key has been found, update key and exit function
            current->value = value;
            return;
        }
        current = current->next;
    }

    // Key not found - create new node and add at head of list
    Node* new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = map->table[insertionIndex];  // Point to old head
    map->table[insertionIndex] = new_node;        // New node becomes head
    map->size++;
}





int ch_get(const ChainingMap* map, const int key, bool* found) {
    const size_t insertionIndex = hash(key, map->capacity);

    // Walk the chain looking for the key
    Node* current = map->table[insertionIndex];
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
    const size_t insertionIndex = hash(key, map->capacity);

    Node* current = map->table[insertionIndex];
    Node* prev = NULL;

    // Walk the chain
    while (current != NULL) {
        if (current->key == key) {
            // Found it - remove from chain
            if (prev == NULL) {
                // Removing head of list
                map->table[insertionIndex] = current->next;
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