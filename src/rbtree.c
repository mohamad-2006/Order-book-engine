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
    if (!new_node) return NULL;
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


void rbtree_destroy(PriceLevel* node) {
    if (node == NULL) return; // NIL si tu utilises des sentinelles

    // Parcours post-ordre : on nettoie les enfants avant le parent
    rbtree_destroy(node->left);
    rbtree_destroy(node->right);
    
    // Si ton Node contient un PriceLevel, libère-le ici
    // free(node->price_level); 
    free(node);
}