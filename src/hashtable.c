#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"

static uint32_t hash_fnv1a(uint64_t key) {
    uint32_t hash = 2166136261u; // FNV offset basis
    for (int i = 0; i < 8; i++) {
        hash ^= (key & 0xFF);
        hash *= 16777619; // FNV prime
        key >>= 8;
    }
    return hash;
}

// Crée une nouvelle table de hachage
HashTable* hashtable_create(uint32_t size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;

    ht->size = size;
    ht->count = 0;
    // calloc initialise les pointeurs à NULL
    ht->table = (Hashnode**)calloc(size, sizeof(Hashnode*));
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    return ht;
}

static void hashtable_resize(HashTable* ht) {
    uint32_t old_size = ht->size;
    Hashnode** old_table = ht->table;

    ht->size *= 2; // Double la taille
    ht->count = 0; // Réinitialise le compteur
    ht->table = (Hashnode**)calloc(ht->size, sizeof(Hashnode*));


    // Réinsérer les éléments dans la nouvelle table
    for (uint32_t i = 0; i < old_size; i++) {
        Hashnode* current = old_table[i];
        while (current != NULL) {
            Hashnode* next = current->next; // Sauvegarde le prochain noeud
            hashtable_insert(ht, current->order);
            free(current);
            current = next;
        }
    }
    free(old_table);
    printf("[Moteur] Rehashing effectue: Nouvelle taille = %u\n", ht->size);
}

bool hashtable_insert(HashTable* ht, Order* order) {
    if (!ht || !order) return false;
    if ((float)(ht->count + 1) / ht->size > HT_LOAD_FACTOR) {
        hashtable_resize(ht);
    }

    uint32_t index = hash_fnv1a(order->id) & (ht->size -1);
    Hashnode* new_node = (Hashnode*)malloc(sizeof(Hashnode));
    if (!new_node) return false;

    new_node->order = order;
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
    ht->count++;
    return true;
}

Order* hashtable_get(HashTable* ht, uint64_t id) {
    if (!ht) return NULL;
    uint32_t index = hash_fnv1a(id) & (ht->size -1);
    Hashnode* current = ht->table[index];
    while (current != NULL) {
        if (current->order->id == id) {
            return current->order;
        }
        current = current->next;
    }
    return NULL; // Non trouvé
}

bool hashtable_remove(HashTable* ht, uint64_t id) {
    if (!ht) return false;
    uint32_t index = hash_fnv1a(id) & (ht->size -1);
    Hashnode* current = ht->table[index];
    Hashnode* prev = NULL;

    while (current != NULL) {
        if (current->order->id == id) {
            if (prev == NULL) {
                ht->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            ht->count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false; // Non trouvé
}

void hashtable_destroy(HashTable* ht) {
    if (!ht) return;
    for (uint32_t i = 0; i < ht->size; i++) {
        Hashnode* current = ht->table[i];
        while (current != NULL) {
            Hashnode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(ht->table);
    free(ht);
}