#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "orderbook.h"

/*Order* create_order(uint64_t id, OrderType type, OrderSide side, Price price, uint32_t quantity) {
    
    //Allocation dynamique de mémoire pour un nouvel ordre
    Order* new_order = (Order*)malloc(sizeof(Order));
    
    
    //Verification de securité pour s'assurer que l'allocation mémoire a réussi
    if (new_order == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour l'ordre.\n");
        return NULL;
    }

    //Initialisation des champs de l'ordre avec les valeurs fournies
    new_order->id = id;
    new_order->type = type;
    new_order->side = side;
    new_order->price = price;
    new_order->quantity = quantity;
    new_order->timestamp = time(NULL); // Stocke l'heure actuelle
    new_order->next = NULL; // Initialise le pointeur next à NULL
    new_order->prev = NULL; // Initialise le pointeur prev à NULL
    return new_order;
}

void free_order(Order* order) {
    if(order != NULL)
    {    
        //Libération de la mémoire allouée pour l'ordre
        free(order);
        printf("Ordre %ld libere avec succes.\n", order->id);
    }

}*/