#include <stdlib.h>
#include <stdio.h>
#include "orderbook.h"
#include "rbtree.h"
#include "trade.h"
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

void match_order(OrderBook* book, Order* incoming) {
    if (incoming->side == BUY) {
        // Un acheteur veut matcher avec les vendeurs les moins chers (asks)
        while (incoming->quantity > 0) {
            PriceLevel* best_ask = rbtree_minimum(book->asks->root);
            if (!best_ask) break; // Plus de vendeurs
            
            if (incoming->price < best_ask->price && incoming->type != MARKET) break;

            Order* limit_order = best_ask->head_orders;
            while (limit_order && incoming->quantity > 0) {
                uint32_t match_qty = (incoming->quantity < limit_order->quantity) ? incoming->quantity : limit_order->quantity;
                
                incoming->quantity -= match_qty;
                limit_order->quantity -= match_qty;
                book->trade_count++; 

                if (limit_order->quantity == 0) {
                    Order* to_free = limit_order;
                    limit_order = limit_order->next;
                    
                    // 🔥 CORRECTION DLL_REMOVE INLINE : On détache l'ordre proprement
                    if (to_free->prev) {
                        to_free->prev->next = to_free->next;
                    } else {
                        best_ask->head_orders = to_free->next; // C'était le premier élément
                    }
                    if (to_free->next) {
                        to_free->next->prev = to_free->prev;
                    }

                    // Note : Si ton projet utilise 'hashtable_delete' ou un autre nom,
                    // adapte cette ligne selon ton include/hashtable.h
                    hashtable_remove(book->order_map, to_free->id); 
                    
                    pool_free(book->pool, to_free);
                } else {
                    limit_order = limit_order->next;
                }
            }

            // Si le niveau de prix est vide, on le supprime de l'arbre
            if (best_ask->head_orders == NULL) {
                // 🔥 CORRECTION : On passe 'best_ask' (le pointeur) au lieu de 'best_ask->price'
                rbtree_delete(book->asks, best_ask);
            }
        }
    } 
    else {
        // Un vendeur veut matcher avec les acheteurs les plus chers (bids)
        while (incoming->quantity > 0) {
            PriceLevel* best_bid = rbtree_maximum(book->bids->root);
            if (!best_bid) break; // Plus d'acheteurs
            
            if (incoming->price > best_bid->price && incoming->type != MARKET) break;

            Order* limit_order = best_bid->head_orders;
            while (limit_order && incoming->quantity > 0) {
                uint32_t match_qty = (incoming->quantity < limit_order->quantity) ? incoming->quantity : limit_order->quantity;
                
                incoming->quantity -= match_qty;
                limit_order->quantity -= match_qty;
                book->trade_count++; 

                if (limit_order->quantity == 0) {
                    Order* to_free = limit_order;
                    limit_order = limit_order->next;
                    
                    // 🔥 CORRECTION DLL_REMOVE INLINE
                    if (to_free->prev) {
                        to_free->prev->next = to_free->next;
                    } else {
                        best_bid->head_orders = to_free->next;
                    }
                    if (to_free->next) {
                        to_free->next->prev = to_free->prev;
                    }

                    hashtable_remove(book->order_map, to_free->id); 
                    
                    pool_free(book->pool, to_free);
                } else {
                    limit_order = limit_order->next;
                }
            }

            if (best_bid->head_orders == NULL) {
                // 🔥 CORRECTION : On passe 'best_bid' directement
                rbtree_delete(book->bids, best_bid);
            }
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

// Fonction utilitaire pour compter la profondeur (nombre de nœuds dans l'arbre)
int get_tree_depth(PriceLevel* node) {
    if (node == NULL) return 0;
    return 1 + get_tree_depth(node->left) + get_tree_depth(node->right);
}

void display_book_state(OrderBook* book) {
    // Extraction des meilleurs prix (Dépend de tes fonctions rbtree_minimum/maximum)
    PriceLevel* best_bid_node = rbtree_maximum(book->bids->root); // Plus cher acheteur
    PriceLevel* best_ask_node = rbtree_minimum(book->asks->root); // Moins cher vendeur

    long best_bid = best_bid_node ? best_bid_node->price : 0;
    long best_ask = best_ask_node ? best_ask_node->price : 0;
    long spread = (best_bid > 0 && best_ask > 0) ? (best_ask - best_bid) : 0;

    int bid_depth = get_tree_depth(book->bids->root);
    int ask_depth = get_tree_depth(book->asks->root);

    printf("\n📊 --- ÉTAT FINAL DU CARNET ---\n");
    printf("   Meilleur Bid (Achat) : %ld\n", best_bid);
    printf("   Meilleur Ask (Vente) : %ld\n", best_ask);
    printf("   Spread Spécifié      : %ld ticks\n", spread);
    printf("   Profondeur des Bids  : %d niveaux de prix\n", bid_depth);
    printf("   Profondeur des Asks  : %d niveaux de prix\n", ask_depth);
    printf("--------------------------------\n");
}