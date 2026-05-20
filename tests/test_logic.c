#include <stdio.h>
#include <assert.h>
#include "orderbook.h"
#include "errors.h"

void test_partial_fill() {
    printf("Test : Exécution partielle... ");
    OrderBook* book = orderbook_create(1000000);
    
    // Vendeur offre 100 à 15000
    orderbook_add_order(book, 1, LIMIT, SELL, 15000, 100);
    // Acheteur demande 40 à 15000 (Cross)
    orderbook_add_order(book, 2, LIMIT, BUY, 15000, 40);
    
    // Vérifications
    assert(book->trade_count == 1);
    
    Order* remaining_ask = hashtable_get(book->order_map, 1);
    assert(remaining_ask != NULL);
    assert(remaining_ask->quantity == 60); // 100 - 40
    
    Order* filled_buy = hashtable_get(book->order_map, 2);
    assert(filled_buy == NULL); // L'ordre 2 a dû être détruit et retiré
    
    orderbook_destroy(book);
    printf("✅ OK\n");
}

void test_cancel_non_existent() {
    printf("Test : Annulation d'un ordre inexistant... ");
    OrderBook* book = orderbook_create(1000000);
    
    EngineStatus status = orderbook_cancel_order(book, 999);
    assert(status == ERR_ORDER_NOT_FOUND);
    
    orderbook_destroy(book);
    printf("✅ OK\n");
}

int main() {
    printf("=== DÉMARRAGE DES TESTS UNITAIRES ===\n");
    test_partial_fill();
    test_cancel_non_existent();
    // Ajoute d'autres tests ici (carnet vide, etc.)
    printf("=== TOUS LES TESTS SONT PASSÉS ! ===\n");
    return 0;
}