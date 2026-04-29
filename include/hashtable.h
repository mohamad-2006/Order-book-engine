#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "order.h"

// Noeud pour gerer les collisions dans la table de hachage
typedef struct Hashnode {
    Order* order;
    struct Hashnode* next; // Pointeur vers le prochain noeud en cas de collision
} Hashnode;

typedef struct HashTable {
    uint32_t size;
    Hashnode** table; // Tableau de listes chaînées pour gérer les collisions
} HashTable;

// Signatures
HashTable* hashtable_create(uint32_t size);
bool hashtable_insert(HashTable* ht, Order* order);
Order* hashtable_get(HashTable* ht, uint64_t id);
bool hashtable_remove(HashTable* ht, uint64_t id);
void hashtable_destroy(HashTable* ht);

#endif