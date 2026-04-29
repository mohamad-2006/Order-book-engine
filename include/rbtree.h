#ifndef RBTREE_H
#define RBTREE_H

#include "order.h"
typedef enum { RED, BLACK } NodeColor;

// Representation d'un niveau de prix (un noeud de l'arbre rouge-noir)

typedef struct PriceLevel {
    Price price;           // Prix du niveau
    uint32_t quantity;     // Quantité totale à ce prix
    Order* head_orders;        // Premier ordre à ce niveau de prix (liste chaînée d'ordres)
    Order* tail_orders;        // Dernier ordre à ce niveau de prix (pour faciliter l'ajout d'ordres)

    // Proprietes de l'arbre rouge-noir
    NodeColor color;       // Couleur du noeud (RED ou BLACK)
    struct PriceLevel* left;   // Fils gauche
    struct PriceLevel* right;  // Fils droit
    struct PriceLevel* parent; // Parent du noeud
} PriceLevel;

// Structure de l'arbre (Pour gerer facilement la racine)
typedef struct RBTree {
    PriceLevel* root;     // Racine de l'arbre
} RBTree;

// Sigantures
RBTree* create_rbtree();
PriceLevel* rbtree_insert(RBTree* tree, Price price);
void rbtree_delete(RBTree* tree, Price price);
PriceLevel* rbtree_find(RBTree* tree, Price price);
PriceLevel* rbtree_minimum(PriceLevel* node);
PriceLevel* rbtree_maximum(PriceLevel* node);

#endif