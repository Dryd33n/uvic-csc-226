#include "hash_tables.h"
#include <stdlib.h>

/* HASH TABLE WITH SEPARATE CHAINING IMPLEMENTATION
 *
 * Credits:
 * This implementation is adapted from the implementation provided from GeeksforGeeks
 * https://www.geeksforgeeks.org/dsa/implementation-of-hash-table-in-c-using-separate-chaining/
 */

// same hash function as linear probing
static size_t hash(int key, size_t capacity){
    unsigned int k = (unsigned int) key;  // make negative keys positive
    return k % capacity;
}



ChainingMap* ch_create(size_t capacity){

    ChainingMap* m = malloc(sizeof(ChainingMap));

    m->capacity = capacity;
    m->size = 0;

    // allocate array of node pointers
    m->table = calloc(capacity, sizeof(Node*));

    return m;
}



void ch_put(ChainingMap* map, int key, int value){

    size_t index = hash(key, map->capacity);

    Node* curr = map->table[index];

    // check if key already exists
    while (curr != NULL){
        if (curr->key == key){
            curr->value = value;   // update existing
            return;
        }
        curr = curr->next;
    }

    // not found -> insert new node at head
    Node* n = malloc(sizeof(Node));
    n->key = key;
    n->value = value;
    n->next = map->table[index];

    map->table[index] = n;
    map->size++;
}



int ch_get(const ChainingMap* map, int key, bool* found){

    size_t index = hash(key, map->capacity);

    Node* curr = map->table[index];

    while (curr != NULL){
        if (curr->key == key){
            *found = true;
            return curr->value;
        }
        curr = curr->next;
    }

    *found = false;
    return 0;
}



bool ch_remove(ChainingMap* map, int key){

    size_t index = hash(key, map->capacity);

    Node* curr = map->table[index];
    Node* prev = NULL;

    while (curr != NULL){

        if (curr->key == key){

            if (prev == NULL){
                // removing first node
                map->table[index] = curr->next;
            } else {
                prev->next = curr->next;
            }

            free(curr);
            map->size--;
            return true;
        }

        prev = curr;
        curr = curr->next;
    }

    return false;  // not found
}



void ch_destroy(ChainingMap* map){

    for (size_t i = 0; i < map->capacity; i++){

        Node* curr = map->table[i];

        while (curr != NULL){
            Node* next = curr->next;
            free(curr);
            curr = next;
        }
    }

    free(map->table);
    free(map);
}