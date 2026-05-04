#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <stddef.h>
#include "order.h"

typedef struct OrderPool
{
    Order* memory_block;
    Order* free_list;
    size_t capacity;
} OrderPool;

OrderPool* pool_create(size_t capacity);
Order* pool_alloc(OrderPool* pool);
void pool_free(OrderPool* pool, Order* order);
void pool_destroy(OrderPool* pool);

#endif
