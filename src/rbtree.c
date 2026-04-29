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