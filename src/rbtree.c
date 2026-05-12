#include <stdlib.h>
#include <stdio.h>
#include "rbtree.h"

// Fonction utilitaire pour allouer un nouveau niveau de prix
PriceLevel* create_price_level(Price price) {
    PriceLevel* node = (PriceLevel*)malloc(sizeof(PriceLevel));
    if (!node) return NULL;

    node->price = price;
    node->quantity = 0;
    node->head_orders = NULL;
    node->tail_orders = NULL;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = RED; // Par défaut, les nouveaux nœuds sont rouges

    return node;
}

RBTree* create_rbtree(){
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    if (tree){
        tree->root = NULL;
    }
    return tree;
}

// Recherche O(log n) d'un niveau de prix dans l'arbre
PriceLevel *rbtree_find(RBTree* tree, Price price) {
    PriceLevel* current = tree->root;
    while (current != NULL) {
        if (price < current->price) {
            current = current->left;
        } else if (price > current->price) {
            current = current->right;
        } else {
            return current; // Niveau de prix trouvé
        }
    }
    return NULL; // Niveau de prix non trouvé
}

// Insertion iterative d'un niveau de prix dans l'arbre
PriceLevel* rbtree_insert(RBTree* tree, Price price) {
    PriceLevel* current = tree->root;
    PriceLevel* parent = NULL;
    while (current != NULL)
    {
        parent = current;
        if (price == current->price) return current;
        if (price < current->price) current = current->left;
        else current = current->right;
    }

    PriceLevel* new_node = create_price_level(price);
    new_node->parent = parent;

    if(parent == NULL)
    {
        tree->root = new_node; // L'arbre était vide
    }
    else if (price < parent->price)
    {
        parent->left = new_node;
    }
    else
    {
        parent->right = new_node;
    }
    return new_node;
}

void rbtree_delete(RBTree* tree, PriceLevel* level){
    if (!tree || !level) return;

    if (level->left == NULL && level->right == NULL){
        if (level->parent){
            if (level->parent->left ==level) level->parent->left =NULL;
            else level->parent->right = NULL;
        } else{
            tree->root = NULL;
        }
        free(level);
    }
    else if (level->left == NULL || level->right == NULL){
        PriceLevel* child = (level->left != NULL) ? level->left : level->right;
        child->parent = level->parent;

        if (level->parent){
            if (level->parent->left == level) level->parent->left = child;
            else level->parent->right = child;
        }
        else{
            tree->root = child;
        }
        free(level);
    }
    else{
        PriceLevel* successor = rbtree_minimum(level->right);

        level->price = successor->price;
        level->quantity = successor->quantity;
        level->head_orders = successor->head_orders;
        level->tail_orders = successor->tail_orders;

        Order* curr_order = level->head_orders;
        while (curr_order){
            curr_order->level = level;
            curr_order = curr_order->next;
        }

        rbtree_delete(tree, successor);
    }
}

// Minimum (Pour le meilleur ASK) - On vas tout a gauche
PriceLevel* rbtree_minimum(PriceLevel* node) {
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Maximum (Pour le meilleur BID) - On vas tout a droite
PriceLevel* rbtree_maximum(PriceLevel* node) {
    while (node && node->right != NULL) {
        node = node->right;
    }
    return node;
}


void dll_push_back(PriceLevel* level, Order* order){
    if (!level || !order) return;
    
    order->level = level;
    order->next = NULL;
    order->prev = level->tail_orders;

    if (level->tail_orders) {
        level->tail_orders->next = order;
    }
    else{
        level->head_orders = order;
    }

    level->tail_orders = order;
    level->quantity += order->quantity;
}

void dll_remove_order(PriceLevel* level, Order* order){
    if (!level || !order) return;

    if (order->prev){
        order->prev->next = order->next;
    }
    else {
        level->head_orders =order->next;
    }

    if (order->next){
        order->next->prev = order->prev;
    }
    else{
        level->tail_orders = order->prev;
    }

    level->quantity -= order->quantity;

    order->next = NULL;
    order->prev = NULL;
    order->level = NULL;
}

Order* dll_front(PriceLevel* level){
    if(!level) return NULL;
    return level->head_orders;
}