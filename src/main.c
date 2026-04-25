#include <stdio.h>
#include "../include/orderbook.h"

int main() {
    // Création d'un ordre de test (Achat de 10 Nasdaq à 18000.50)
    Order* my_order = create_order(1, LIMIT, BUY, 18000.50, 10);

    if (my_order != NULL) {
        printf("Ordre créé : ID %d | Prix: %.2f | Quantité: %d\n", 
                my_order->id, my_order->price, my_order->quantity);
        
        // Libération de la mémoire
        free_order(my_order);
    }

    return 0;
}