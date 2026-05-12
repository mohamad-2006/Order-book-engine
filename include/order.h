#ifndef ORDER_H
#define ORDER_H

#include "types.h"
#include <stdint.h>

typedef long Price;

struct PriceLevel;


typedef struct Order {
    uint64_t id;
    OrderType type;
    OrderSide side;
    Price price;
    uint32_t quantity;
    struct PriceLevel* level;
    
    TIMESTAMP timestamp;      // Stocke l'heure de création
    struct Order* next;    // Pour la future liste chaînée
    struct Order* prev;    // Pour la future liste chaînée   
} Order;

Order* create_order(uint64_t id, OrderType type, OrderSide side, Price price, uint32_t quantity);
void free_order(Order* order);
#endif