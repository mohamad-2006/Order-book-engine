#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "rbtree.h"

void test_bst_operations(){
    RBTree tree = { .root = NULL};

    // Test d'insertion
    srand(time(NULL));
    Price expected_min = 9999999;
    Price expected_max = 0;

    for(int i = 0; i < 100; i++){
        // Prix entre 10000 et 20000
        Price p = 10000 + rand() % 10001;
        rbtree_insert(&tree, p);

        if (p < expected_min) expected_min = p;
        if (p > expected_max) expected_max = p;
    }
    
    // Verification des extremums
    PriceLevel* min_node = rbtree_minimum(tree.root);
    PriceLevel* max_node = rbtree_maximum(tree.root);

    assert(min_node->price == expected_min);
    assert(max_node->price == expected_max);

    printf("Test BST : 100 insertions réussies.\n");
    printf("Test BST : Min (Meilleur ASK) = %ld\n", min_node->price);
    printf("Test BST : Max (Meilleur BID) = %ld\n", max_node->price);


    // Test de recherche
    Price test_price = 15000;
    rbtree_insert(&tree, test_price); // Assure que le prix existe
    PriceLevel* found = rbtree_find(&tree, test_price);
    assert(found != NULL && found->price == test_price);
    printf("Test BST : Recherche du prix %ld réussie.\n", test_price);
}

int main() {
    test_bst_operations();
    printf("\nFélicitations : Tous les tests BST sont passés !\n");
    return 0;
}