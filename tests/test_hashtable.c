#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "order.h"
#include <assert.h>
#include <time.h>

#define TIME_DIFF(start,end) \
    ((double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1E9)

int main() {
    uint32_t n = 100000; // Nombre d'ordres à insérer
    HashTable* ht = hashtable_create(HT_INITIAL_SIZE);
    if (!ht) {
        fprintf(stderr, "Erreur de création de la table de hachage\n");
        return EXIT_FAILURE;
    }

    Order* orders_pool = calloc(n, sizeof(Order));
    for(uint32_t i =0; i < n; i++)
    {
        orders_pool[i].id = i;
        orders_pool[i].quantity = 10;
    }

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (uint32_t i = 0; i < n; i++) {
        hashtable_insert(ht, &orders_pool[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Insertion de %u ordres: %.6f secondes\n", n, TIME_DIFF(start, end));

    // Test recherche
    uint64_t checkum = 0;
    uint32_t count = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (uint32_t i = 0; i < n; i++) {
        Order* o = hashtable_get(ht, i);
        if (o) {
            checkum += o->id;
            count++;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Recherche de %u ordres: %.6f secondes\n", n, TIME_DIFF(start, end));
    printf("Checksum: %lu, Nombre d'ordres trouvés: %u\n", checkum, count);

    hashtable_destroy(ht);
    free(orders_pool);

    return 0;
}