#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "rbtree.h"
#include "hashtable.h"
#include "order.h"
#include "mempool.h"
#include <stdint.h>
#include <stddef.h>

typedef struct OrderBook {
    RBTree* bids;          // Arbre des acheteurs (trié décroissant idéalement)
    RBTree* asks;          // Arbre des vendeurs (trié croissant)
    HashTable* order_map;  // Pour trouver un ordre en O(1) via son ID
    OrderPool* pool;
} OrderBook;

// Signatures
OrderBook* orderbook_create(size_t max_orders);

// Les actions principales du moteur
void orderbook_add_order(OrderBook* book, uint64_t order_id, OrderSide side, long price, uint32_t quantity);
void orderbook_cancel_order(OrderBook* book, uint64_t order_id); // Lance l'algorithme d'appariement

#endif // ORDERBOOK_H