#ifndef HASH_TABLES_H
#define HASH_TABLES_H

#include <stddef.h>
#include <stdbool.h>

//EACH HASHTABLE IMPLEMENTATION HAS THE FOLLOWING FUNCTIONS:
/*
 *  1. Create
 *  2. Put
 *  3. Get
 *  4. Remove
 *  5. Destroy
 */



//LINEAR PROBING
typedef struct {
    int* keys;
    int* values;
    size_t capacity;
    size_t size;
} LinearProbingMap;

LinearProbingMap* lp_create(size_t capacity);
void lp_put(LinearProbingMap* map, int key, int value);
int lp_get(const LinearProbingMap* map, int key, bool* found);
bool lp_remove(LinearProbingMap* map, int key);
void lp_destroy(LinearProbingMap* map);


//SEPERATE CHAINING
typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node** table;
    size_t capacity;
    size_t size;
} ChainingMap;


ChainingMap* ch_create(size_t capacity);
void ch_put(ChainingMap* map, int key, int value);
int ch_get(const ChainingMap* map, int key, bool* found);
bool ch_remove(ChainingMap* map, int key);
void ch_destroy(ChainingMap* map);

#endif // HASH_TABLES_H