#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "order.h"
#include <stdint.h>
#include <stdbool.h>

#define HT_INITIAL_SIZE 65536 // Taille initiale de la table de hachage
#define HT_LOAD_FACTOR 0.75 // Facteur de charge pour redimensionner la table

// Noeud pour gerer les collisions dans la table de hachage
typedef struct Hashnode {
    Order* order;
    struct Hashnode* next; // Pointeur vers le prochain noeud en cas de collision
} Hashnode;

typedef struct HashTable {
    uint32_t size;
    uint32_t count;
    Hashnode** table; // Tableau de listes chaînées pour gérer les collisions
} HashTable;

// Signatures
HashTable* hashtable_create(uint32_t size);
bool hashtable_insert(HashTable* ht, Order* order);
Order* hashtable_get(HashTable* ht, uint64_t id);
bool hashtable_remove(HashTable* ht, uint64_t id);
void hashtable_destroy(HashTable* ht);

#endif