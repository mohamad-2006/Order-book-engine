#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "order.h"
#include <assert.h>
#include <time.h>

#define TIME_DIFF(start,end) \
    ((double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1E9)

void test_million_orders(){
    printf("Test: Insertion de 1 million d'ordres\n");
    HashTable* ht = hashtable_create(HT_INITIAL_SIZE);
    int num_orders = 1000000;

    // On simule un tableau de pointeurs vers des ordres fictifs
    Order** fake_orders = malloc(num_orders * sizeof(Order*));

    struct timespec start, end;

    // 1. Test d'insertion
    clock_gettime(CLOCKS_PER_SEC, &start);
    uint64_t total_qty_check = 0;
    for(int i =0; i < num_orders; i++){
        fake_orders[i] = malloc(sizeof(Order));
        fake_orders[i]->id = 1000000 + i; // IDs unique
       hashtable_insert(ht, fake_orders[i]);
       Order* o = hashtable_get(ht, 1000000 + i);
        if (o) {
            total_qty_check += o->quantity;
        }
    }

    clock_gettime(CLOCKS_PER_SEC, &end);
    printf("Insertion de 1 million d'ordres: %.8f secondes\n", TIME_DIFF(start,end));
    printf("Vérification : %lu ordres traités\n", total_qty_check);

    // 2. Test de récupération
    clock_gettime(CLOCKS_PER_SEC, &start);

    Order* found = hashtable_get(ht, 1000000 + num_orders - 1);
    clock_gettime(CLOCKS_PER_SEC, &end);    

    assert(found != NULL);
    assert(found->id == (uint64_t)(1000000 + num_orders - 1));
    printf("Récupération d'un ordre: %.8f secondes\n", TIME_DIFF(start, end));


    // 3. Nettoyage
    for(int i = 0; i < num_orders; i++){
        free(fake_orders[i]);
    }
    free(fake_orders);
    hashtable_destroy(ht);
}


int main() {
    test_million_orders();
    return 0;
}