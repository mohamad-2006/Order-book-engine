#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "rbtree.h"
#include "hashtable.h"

typedef struct OrderBook {
    RBTree* bids;          // Arbre des acheteurs (trié décroissant idéalement)
    RBTree* asks;          // Arbre des vendeurs (trié croissant)
    HashTable* order_map;  // Pour trouver un ordre en O(1) via son ID
} OrderBook;

// Signatures
OrderBook* orderbook_create();
void orderbook_destroy(OrderBook* book);

// Les actions principales du moteur
bool orderbook_add(OrderBook* book, Order* order);
bool orderbook_cancel(OrderBook* book, uint64_t order_id);
void orderbook_match(OrderBook* book); // Lance l'algorithme d'appariement

#endif // ORDERBOOK_H