#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "../include/orderbook.h"

// Structure globale pour le rapport de performance
typedef struct 
{
    uint32_t total_orders;
    uint32_t total_trades;
    double csv_load_time;
    double matching_time;
    double total_time;
} EngineStats;

//
bool verbose_mode = false;
bool stats_mode = false;

// Siganture de la fonction d'affichage
void display_book_state(OrderBook* book);
void run_benchmark(OrderBook* book, const char* filepath, EngineStats* stats);


int main(int argc, char* argv[]) {
    char* input_filepath = NULL;

    // Simple portable CLI parsing (avoids getopt dependency)
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) {
                input_filepath = argv[++i];
            } else {
                fprintf(stderr, "❌ Erreur : --input requiert un argument.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            verbose_mode = true;
        } else if (strcmp(argv[i], "--stats") == 0 || strcmp(argv[i], "-s") == 0) {
            stats_mode = true;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s --input <file.csv> [--verbose] [--stats]\n", argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    if (!input_filepath) {
        fprintf(stderr, "❌ Erreur : L'option --input <fichier.csv> est obligatoire.\n");
        return 1;
    }

    // Allocation du moteur (Ajusté à 1.5M pour encaisser le benchmark)
    OrderBook* book = orderbook_create(1500000);
    EngineStats stats = {0};

    printf("🚀 Initialisation du moteur...\n");
    run_benchmark(book, input_filepath, &stats);

    // Nettoyage
    orderbook_destroy(book);
    return 0;
}