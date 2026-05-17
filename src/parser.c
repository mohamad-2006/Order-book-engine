#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "orderbook.h"

#define TIME_DIFF(start, end)\
    ((double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1E9)

// Parsing d'entier ultra-rapide
static inline uint64_t parse_uint64(char** cursor) {
    uint64_t res = 0;
    while (**cursor >= '0' && **cursor <= '9') {
        res = (res * 10) + (**cursor - '0');
        (*cursor)++;
    }
    return res;
}

void load_and_match_csv(OrderBook* book, const char* filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier CSV");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Erreur fstat");
        close(fd);
        return;
    }

    // Mapping du fichier en memoire
    char* data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("Erreur mmap");
        close(fd);
        return;
    }
    
    char* cursor = data;
    char* end = data + sb.st_size;

    // Ignorer la ligne d'en-tete du CSV
    while (cursor < end && *cursor != '\n') {
        cursor++;
    }
    if (cursor < end) cursor++;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    uint32_t parsed_count = 0;

    // Boucle de parsing caractère par caractère
    while (cursor < end) {
        // Format attendu : order_id, side, type, price, quantity, timestamp\n
        uint64_t id = parse_uint64(&cursor);
        cursor++; // sauter la virgule ','

        OrderSide side = (OrderSide)parse_uint64(&cursor);
        cursor++;

        OrderType type = (OrderType)parse_uint64(&cursor);
        cursor++;

        long price = (long)parse_uint64(&cursor);
        cursor++;

        uint32_t quantity = (uint32_t)parse_uint64(&cursor);
        cursor++;

        uint64_t ts = parse_uint64(&cursor);

        // Gérer le retour à la ligne (Windows \r\n ou Linux \n)
        if (cursor < end && *cursor == '\r') cursor++;
        if (cursor < end && *cursor == '\n') cursor++;

        // 3. Injection directe dans le moteur de matching
        // (Aucun malloc n'est fait ici, tout part dans le Mempool !)
        orderbook_add_order(book, id, type, side, price, quantity);
        parsed_count++;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    printf("🏁 Parsing & Matching terminé !\n");
    printf("   Ordres traités : %u\n", parsed_count);
    printf("   Temps total    : %.6f secondes\n", TIME_DIFF(start_time, end_time));
    printf("   Vitesse        : %.0f ordres/sec\n", parsed_count / TIME_DIFF(start_time, end_time));

    // Nettoyage de la mémoire virtuelle
    munmap(data, sb.st_size);
    close(fd);
    
}


