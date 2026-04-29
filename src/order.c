#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "order.h"

Order* create_order(uint64_t id, OrderType type, OrderSide side, Price price, uint32_t quantity) {
    Order* new_order = (Order*)malloc(sizeof(Order));
    if (!new_order) return NULL;

    new_order->id = id;
    new_order->side = side;
    new_order->type = type;
    new_order->price = price;
    new_order->quantity = quantity;
    new_order->timestamp = time(NULL);
    new_order->prev = NULL;
    new_order->next = NULL;

    return new_order;
}

void free_order(Order* order) {
    free(order);
}