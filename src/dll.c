#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

// Ajoute un ordre à la fin de la file (Priority Queue)
void dll_push_back(PriceLevel* level, Order* order) {
    if (!level || !order) return;
    
    order->level = level;
    order->next = NULL;
    order->prev = level->tail_orders;

    if (level->tail_orders) {
        level->tail_orders->next = order;
    } else {
        level->head_orders = order;
    }

    level->tail_orders = order;
    level->quantity += order->quantity;
}

// Retire un ordre specifique (pour annulation ou Matching complet)
void dll_remove_order(PriceLevel* level, Order* order) {
    if (!level || !order) return;

    if (order->prev) {
        order->prev->next = order->next;
    } else {
        level->head_orders = order->next;
    }

    if (order->next) {
        order->next->prev = order->prev;
    } else {
        level->tail_orders = order->prev;
    }

    level->quantity -= order->quantity;

    order->next = NULL;
    order->prev = NULL;
    order->level = NULL;
}

// Recupere l'ordre de priorité (le premier de la file) pour le matching
Order* dll_front(PriceLevel* level) {
    if (!level) return NULL;
    return level->head_orders;
}