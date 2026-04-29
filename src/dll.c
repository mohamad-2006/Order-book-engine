#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

// Ajoute un ordre à la fin de la file (Priority Queue)
void dll_push_back(PriceLevel* level, Order* order){
    if (level->head_orders == NULL) {
        // Si la file est vide, le nouvel ordre devient à la fois la tête et la queue
        level->head_orders = order;
        level->tail_orders = order;
    } else {
        // Sinon, ajoute le nouvel ordre à la fin de la file
        level->tail_orders->next = order; // L'ancien dernier ordre pointe vers le nouveau
        order->prev = level->tail_orders; // Le nouveau ordre pointe vers l'ancien dernier
        level->tail_orders = order; // Le nouveau ordre devient le dernier de la file
        order->next = NULL; // Le nouveau dernier ordre ne pointe vers rien
    }
}

// Retire un ordre specifique (pour annulation ou Matching complet)
void dll_remove_order(PriceLevel* level, Order* order){
    if (level == NULL || order == NULL) return;

    if (order->prev != NULL){
        order->prev->next = order->next; // L'ordre précédent pointe vers l'ordre suivant
    } else{
        // Si l'ordre à supprimer est la tête de la file, on met à jour la tête
        level->head_orders = order->next;
    }

    if (order->next != NULL){
        order->next->prev = order->prev; // L'ordre suivant pointe vers l'ordre précédent
    } else{
        // Si l'ordre à supprimer est la queue de la file, on met à jour la queue
        level->tail_orders = order->prev;
    }

    level->quantity -= order->quantity; // Met à jour la quantité totale du niveau de prix
    order->next = NULL; // Nettoie les pointeurs de l'ordre supprimé
    order->prev = NULL;
}


// Recupere l'ordre de priorité (le premier de la file) pour le matching
Order* dll_front(PriceLevel* level){
    return (level != NULL) ? level->head_orders :NULL;
}