#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "rbtree.h"
#include "hashtable.h"
#include "order.h"
#include "mempool.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


// La structure pour stocker nos métriques de performance
typedef struct {
    uint32_t total_orders;
    uint32_t total_trades;
    double csv_load_time;
    double matching_time;
    double total_time;
} EngineStats;


typedef struct OrderBook {
    RBTree* bids;          // Arbre des acheteurs (trié décroissant idéalement)
    RBTree* asks;          // Arbre des vendeurs (trié croissant)
    HashTable* order_map;  // Pour trouver un ordre en O(1) via son ID
    OrderPool* pool;
    uint32_t trade_count;
} OrderBook;

// Signatures
OrderBook* orderbook_create(size_t max_orders);

// Les actions principales du moteur
void orderbook_add_order(OrderBook* book, uint64_t order_id, OrderType type, OrderSide side, long price, uint32_t quantity);
void orderbook_cancel_order(OrderBook* book, uint64_t order_id); // Lance l'algorithme d'appariement
void load_and_match_csv(OrderBook* book, const char* filepath, EngineStats* stats, bool verbose);
void orderbook_destroy(OrderBook* book);

#endif // ORDERBOOK_H