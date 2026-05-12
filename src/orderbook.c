#include <stdlib.h>
#include <stdio.h>
#include "orderbook.h"
#include "rbtree.h"
#include <time.h>

OrderBook* orderbook_create(size_t max_orders) {
    OrderBook* book = calloc(1, sizeof(OrderBook));
    if (!book) return NULL;

    book->bids = create_rbtree();
    book->asks = create_rbtree();
    book->order_map = hashtable_create(HT_INITIAL_SIZE);
    book->pool = pool_create(max_orders);

    return book;
}

void orderbook_add_order(OrderBook* book, uint64_t id, OrderSide side, long price, uint32_t quantity) {
    // 1. Allocation ultra-rapide
    Order* order = pool_alloc(book->pool);
    if (!order) return;

    // 2. Remplissage correct de TOUTES les données (Le bug du prix à 0 est corrigé ici)
    order->id = id;
    order->side = side;
    order->price = price;
    order->quantity = quantity;
    order->type = LIMIT;
    order->timestamp = time(NULL);
    order->level = NULL;
    order->next = NULL;
    order->prev = NULL;

    // 3. Détermination de l'arbre cible
    RBTree* tree = (side == BUY) ? book->bids : book->asks;

    // 4. Insertion dans le BON arbre (Le bug du hardcoding est corrigé ici)
    PriceLevel* level = rbtree_insert(tree, price);
    
    // 5. Liaison de l'ordre à son niveau
    order->level = level;

    // 6. Sauvegarde dans les structures de données
    hashtable_insert(book->order_map, order);
    dll_push_back(level, order);
}

void orderbook_cancel_order(OrderBook* book, uint64_t id) {
    Order* order = hashtable_get(book->order_map, id);
    if (!order) return;

    PriceLevel* level = order->level;
    
    // Retrait des structures
    hashtable_remove(book->order_map, id);
    dll_remove_order(level, order);

    // Si le niveau de prix est vide, on le nettoie
    if (level->head_orders == NULL) {
        RBTree* tree = (order->side == BUY) ? book->bids : book->asks;
        rbtree_delete(tree, level);
    }

    // Recyclage de la mémoire
    pool_free(book->pool, order);
}