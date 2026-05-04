#include <stdlib.h>
#include <stdio.h>
#include "mempool.h"

OrderPool* pool_create(size_t capacity) {
    // calloc sécurise la mémoire en la remplissant de zéros
    OrderPool* pool = calloc(1, sizeof(OrderPool));
    if (!pool) return NULL;

    pool->capacity = capacity;
    
    // Allocation du bloc massif avec calloc
    pool->memory_block = calloc(capacity, sizeof(Order));
    if (!pool->memory_block) {
        free(pool);
        return NULL;
    }

    // Chaînage de la free list
    for (size_t i = 0; i < capacity - 1; i++) {
        pool->memory_block[i].next = &pool->memory_block[i + 1];
    }
    pool->memory_block[capacity - 1].next = NULL; 
    
    pool->free_list = &pool->memory_block[0];

    return pool;
}

Order* pool_alloc(OrderPool* pool) {
    if (!pool || !pool->free_list) return NULL; 
    
    Order* order = pool->free_list;
    pool->free_list = order->next; 
    
    order->next = NULL;
    order->prev = NULL;
    
    return order;
}

void pool_free(OrderPool* pool, Order* order) {
    if (!pool || !order) return;
    
    order->next = pool->free_list;
    pool->free_list = order;
}

void pool_destroy(OrderPool* pool) {
    if (!pool) return;
    if (pool->memory_block) free(pool->memory_block);
    free(pool);
}