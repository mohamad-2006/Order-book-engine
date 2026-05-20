#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include "orderbook.h"

int main(int argc, char* argv[]) {
    char* input_filepath = NULL;
    bool verbose_mode = false;
    bool stats_mode = false;

    // Configuration des arguments du terminal
    static struct option long_options[] = {
        {"input",   required_argument, 0, 'i'},
        {"verbose", no_argument,       0, 'v'},
        {"stats",   no_argument,       0, 's'},
        {"help",    no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "i:vsh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'i': input_filepath = optarg; break;
            case 'v': verbose_mode = true; break;
            case 's': stats_mode = true; break;
            case 'h':
            default:
                printf("Usage: %s --input <file.csv> [--verbose] [--stats]\n", argv[0]);
                return (opt == 'h') ? 0 : 1;
        }
    }

    // Si on a oublié de donner un fichier, on quitte.
    if (!input_filepath) {
        fprintf(stderr, "❌ Erreur : L'option --input <fichier.csv> est obligatoire.\n");
        return 1;
    }

    // Création du carnet
    OrderBook* book = orderbook_create(1000000); // 1 Million d'ordres
    EngineStats stats = {0};

    printf("🚀 Lancement du moteur sur '%s'...\n", input_filepath);
    
    // Appel de la fonction de notre parser.c
    load_and_match_csv(book, input_filepath, &stats, verbose_mode);

    // Affichage du tableau de bord si --stats est demandé
    if (stats_mode) {
        printf("\n⏱️  --- RAPPORT DE PERFORMANCE ---\n");
        printf("   Ordres traités       : %u\n", stats.total_orders);
        printf("   Trades générés       : %u\n", stats.total_trades);
        printf("   Temps Chargement CSV : %.6f secondes\n", stats.csv_load_time);
        printf("   Temps Matching Pur   : %.6f secondes\n", stats.matching_time);
        printf("   Temps Total Exec     : %.6f secondes\n", stats.total_time);
        if (stats.matching_time > 0) {
            printf("   Débit Matching       : %.0f ordres/sec\n", stats.total_orders / stats.matching_time);
        }
        printf("---------------------------------\n");
    }

    orderbook_destroy(book);
    return 0;
}