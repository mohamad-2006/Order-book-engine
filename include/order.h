#ifndef ORDER_H
#define ORDER_H

#include "types.h"

typedef struct Order {
    uint64_t id;
    OrderType type;
    OrderSide side;
    Price price;
    uint32_t quantity;
    TIMESTAMP timestamp;      // Stocke l'heure de création
    struct Order* next;    // Pour la future liste chaînée
    struct Order* prev;    // Pour la future liste chaînée   
} Order;

Order* create_order(uint64_t id, OrderType type, OrderSide side, Price price, uint32_t quantity);
void free_order(Order* order);
#endif