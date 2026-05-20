#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
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

    // Defintion des options du CLI
    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"verdose", no_argument,     0, 'v'},
        {"stats",   no_argument,     0, 's'},
        {"help",    no_argument,     0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "i:vsh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i':
                input_filepath = optarg;
                break;
            case 'v':
                verbose_mode = true;
                break;
            case 's':
                stats_mode = true;
                break;
            case 'h':
            default:
                printf("Usage: %s --input <file.csv> [--verbose] [--stats]\n", argv[0]);
                return (opt == 'h') ? 0 : 1;
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