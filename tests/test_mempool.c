#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mempool.h"

#define BATCH_SIZE 100000
#define ITERATIONS 10

void benchmark_malloc() {
    Order** arr = calloc(BATCH_SIZE, sizeof(Order*));
    if (!arr) return;

    clock_t start = clock(); // Nouvelle horloge super stable
    
    for (int it = 0; it < ITERATIONS; it++) {
        for (int i = 0; i < BATCH_SIZE; i++) {
            arr[i] = malloc(sizeof(Order));
            if (arr[i]) arr[i]->id = i; // Protection anti-segfault
        }
        for (int i = 0; i < BATCH_SIZE; i++) {
            if (arr[i]) free(arr[i]);
        }
    }
    
    clock_t end = clock();
    double temps_sec = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Standard malloc/free : %.6f secondes\n", temps_sec);
    
    free(arr);
}

void benchmark_mempool() {
    OrderPool* pool = pool_create(BATCH_SIZE);
    if (!pool) {
        printf("Erreur : Impossible de créer le pool.\n");
        return;
    }
    
    Order** arr = calloc(BATCH_SIZE, sizeof(Order*));
    if (!arr) return;

    clock_t start = clock();
    
    for (int it = 0; it < ITERATIONS; it++) {
        for (int i = 0; i < BATCH_SIZE; i++) {
            arr[i] = pool_alloc(pool);
            if (arr[i]) arr[i]->id = i; // Protection anti-segfault
        }
        for (int i = 0; i < BATCH_SIZE; i++) {
            pool_free(pool, arr[i]);
        }
    }
    
    clock_t end = clock();
    double temps_sec = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Memory Pool alloc/free : %.6f secondes\n", temps_sec);
    
    pool_destroy(pool);
    free(arr);
}

int main() {
    printf("--- Benchmark Allocation : 1 Million d'Opérations ---\n");
    benchmark_malloc();
    benchmark_mempool();
    return 0;
}