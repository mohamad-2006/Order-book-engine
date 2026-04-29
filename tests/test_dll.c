#include <assert.h>
#include <stdio.h>
#include "order.h"
#include "rbtree.h"

void test_dll() {
    // Création d'un niveau de prix
    PriceLevel level = { .price = 100, .quantity = 0, .head_orders = NULL, .tail_orders = NULL };

    // Création de quelques ordres
    Order* order1 = create_order(1, LIMIT, BUY, 100, 10);
    Order* order2 = create_order(2, LIMIT, BUY, 100, 20);
    Order* order3 = create_order(3, LIMIT, BUY, 100, 30);

    // Test de l'ajout d'ordres à la file
    dll_push_back(&level, order1);
    dll_push_back(&level, order2);
    dll_push_back(&level, order3);

    assert(level.head_orders == order1);
    assert(level.tail_orders == order3);
    assert(order1->next == order2);
    assert(order2->next == order3);
    assert(order3->next == NULL);

    // Test de la suppression d'un ordre du milieu
    dll_remove_order(&level, order2);
    assert(level.head_orders == order1);
    assert(level.tail_orders == order3);
    assert(order1->next == order3);
    assert(order3->next == NULL);

    // Test de la suppression de la tête
    dll_remove_order(&level, order1);
    assert(level.head_orders == order3);
    assert(level.tail_orders == order3);
    assert(order3->next == NULL);

    // Test de la suppression de la queue
    dll_remove_order(&level, order3);
    assert(level.head_orders == NULL);
    assert(level.tail_orders == NULL);

    printf("Tous les tests DLL ont réussi !\n");
}


int main() {
    test_dll();
    printf("\nFélicitations : Tous les tests DLL sont passés !\n");
    return 0;
}