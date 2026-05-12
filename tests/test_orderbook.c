#include <stdlib.h>
#include "orderbook.h"
#include <assert.h>
#include <stdio.h>


void test_full_cycle() {
    printf("Test d'intégration : Cycle complet... ");
    OrderBook* book = orderbook_create(1000);

    // 1. Ajout d'ordres (via process_order)
    orderbook_add_order(book, 1, BUY, 15000, 10);
    orderbook_add_order(book, 2, BUY, 15000, 5);
    orderbook_add_order(book, 3, SELL, 15100, 20);

    // 2. Vérification de la HashTable
    Order* o1 = hashtable_get(book->order_map, 1);
    assert(o1 != NULL);
    assert(o1->price == 15000);

    assert(o1->level != NULL);
    assert(o1->level->price == 15000);

    // 3. Annulation (via cancel_order)
    orderbook_cancel_order(book, 1);
    assert(hashtable_get(book->order_map, 1) == NULL);

    printf("SUCCÈS\n");
    
    // Si tu as une fonction orderbook_destroy, appelle-la ici, sinon libère juste le book
    free(book);
}

int main() {
    test_full_cycle();
    return 0;
}