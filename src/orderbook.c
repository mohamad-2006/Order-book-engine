#include <stdlib.h>
#include <stdio.h>
#include "orderbook.h"
#include "rbtree.h"
#include "trade.h"
#include <time.h>

//Fonction utilitaire simple pour afficher le trade
/*static void log_trade(uint64_t buyer_id, uint64_t seller_id, Price price, uint32_t qty) {
    // Plus tard, on écrira ça dans un fichier CSV ou binaire pour la vitesse
    printf("⚡ TRADE | Achat #%lu <> Vente #%lu | Prix: %ld | Qty: %u\n", 
           buyer_id, seller_id, price, qty);
}*/

OrderBook* orderbook_create(size_t max_orders) {
    OrderBook* book = calloc(1, sizeof(OrderBook));
    if (!book) return NULL;

    book->bids = create_rbtree();
    book->asks = create_rbtree();
    book->order_map = hashtable_create(HT_INITIAL_SIZE);
    book->pool = pool_create(max_orders);

    return book;
}

void match_order(OrderBook* book, Order* incoming) {
    if (!book || !incoming) return;

    RBTree* opposite_tree = (incoming->side == BUY) ? book->asks : book->bids;

    while (incoming->quantity > 0) {

        // 1. Trouver meilleur prix opposé
        PriceLevel* best_level = (incoming->side == BUY)
            ? rbtree_minimum(opposite_tree->root)   // meilleur ASK
            : rbtree_maximum(opposite_tree->root);  // meilleur BID

        if (!best_level) break;

        // 2. Vérifier condition de match
        int match = 0;

        if (incoming->type == MARKET) {
            match = 1;
        } else if (incoming->side == BUY && incoming->price >= best_level->price) {
            match = 1;
        } else if (incoming->side == SELL && incoming->price <= best_level->price) {
            match = 1;
        }

        if (!match) break;

        // 3. Prendre le premier ordre FIFO
        Order* resting = dll_front(best_level);
        if (!resting) break;

        // 4. Calcul quantité exécutée
        uint32_t traded_qty = (incoming->quantity < resting->quantity)
            ? incoming->quantity
            : resting->quantity;

        // 5. Créer trade
        Trade trade;
        trade.buyer_id  = (incoming->side == BUY) ? incoming->id : resting->id;
        trade.seller_id = (incoming->side == SELL) ? incoming->id : resting->id;
        trade.price     = best_level->price;
        trade.quantity  = traded_qty;
        trade.timestamp = time(NULL);

        /*printf("TRADE: BUYER=%lu SELLER=%lu PRICE=%ld QTY=%u\n",
               trade.buyer_id, trade.seller_id, trade.price, trade.quantity);*/

        // 6. Update quantités
        incoming->quantity -= traded_qty;
        resting->quantity -= traded_qty;
        best_level->quantity -= traded_qty;

        // 7. Si ordre du carnet fini
        if (resting->quantity == 0) {
            dll_remove_order(best_level, resting);
            hashtable_remove(book->order_map, resting->id);
            pool_free(book->pool, resting);
        }

        // 8. Si niveau vide → delete
        if (best_level->head_orders == NULL) {
            rbtree_delete(opposite_tree, best_level);
        }
    }
}

void orderbook_add_order(OrderBook* book, uint64_t id, OrderType type, OrderSide side, long price, uint32_t quantity) {

    Order* order = pool_alloc(book->pool);
    if (!order) return;

    order->id = id;
    order->side = side;
    order->price = price;
    order->quantity = quantity;
    order->type = (price == 0) ? MARKET : LIMIT;
    order->timestamp = time(NULL);
    order->level = NULL;
    order->next = NULL;
    order->prev = NULL;

    // 🔥 MATCHING
    match_order(book, order);

    // Si totalement exécuté → stop
    if (order->quantity == 0) {
        pool_free(book->pool, order);
        return;
    }

    // Sinon → insertion dans carnet
    RBTree* tree = (side == BUY) ? book->bids : book->asks;
    PriceLevel* level = rbtree_insert(tree, price);

    dll_push_back(level, order);
    hashtable_insert(book->order_map, order);
}

void orderbook_cancel_order(OrderBook* book, uint64_t id) {
    Order* order = hashtable_get(book->order_map, id);
    if (!order) return;

    PriceLevel* level = order->level;
    
    // Retrait des structures
    hashtable_remove(book->order_map, id);
    dll_remove_order(level, order);

    // Si le niveau de prix est vide, on le nettoie
    if (level->quantity == 0) {
        RBTree* tree = (order->side == BUY) ? book->bids : book->asks;
        rbtree_delete(tree, level);
    }

    // Recyclage de la mémoire
    pool_free(book->pool, order);
}

void orderbook_destroy(OrderBook* book) {
    if (!book) return;

    // 1. Libérer la HashTable (et ses HashNodes internes)
    if (book->order_map) {
        hashtable_destroy(book->order_map);
    }

    // 2. Libérer le Memory Pool (le gros bloc de 64 Mo)
    if (book->pool) {
        pool_destroy(book->pool);
    }

    // 3. Libérer les arbres (et tous les PriceLevels restants)
    // ⚠️ Attention : il faut une fonction rbtree_destroy qui parcourt l'arbre !
    if (book->bids) {
        rbtree_destroy(book->bids->root); // Fonction récursive à créer
        free(book->bids);
    }
    if (book->asks) {
        rbtree_destroy(book->asks->root);
        free(book->asks);
    }

    // 4. Enfin, libérer la structure principale
    free(book);
}