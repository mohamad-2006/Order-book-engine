#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include "orderbook.h"

#define TIME_DIFF(start, end) \
    ((double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1E9)

// Structure temporaire pour stocker les ordres lus avant de les matcher
typedef struct {
    uint64_t id;
    OrderSide side;
    OrderType type;
    long price;
    uint32_t qty;
} TempOrder;

static inline uint64_t parse_uint64(char** cursor) {
    uint64_t res = 0;
    while (**cursor >= '0' && **cursor <= '9') {
        res = (res * 10) + (**cursor - '0');
        (*cursor)++;
    }
    return res;
}

void load_and_match_csv(OrderBook* book, const char* filepath, EngineStats* stats, bool verbose) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier CSV");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Erreur fstat");
        close(fd); return;
    }

    char* data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("Erreur mmap");
        close(fd); return;
    }

    char* cursor = data;
    char* end = data + sb.st_size;

    // Sauter l'en-tête
    while (cursor < end && *cursor != '\n') cursor++;
    if (cursor < end) cursor++;

    // On prépare un tableau pour stocker 1 Million d'ordres
    uint32_t max_orders = 1000000;
    TempOrder* buffer = malloc(max_orders * sizeof(TempOrder));
    
    struct timespec t0, t1, t2, t3;
    uint32_t parsed_count = 0;

    // ==========================================
    // PHASE 1 : LECTURE DU CSV (PARSING)
    // ==========================================
    clock_gettime(CLOCK_MONOTONIC, &t0);

    while (cursor < end && parsed_count < max_orders) {
        buffer[parsed_count].id = parse_uint64(&cursor); cursor++;
        buffer[parsed_count].side = (OrderSide)parse_uint64(&cursor); cursor++;
        buffer[parsed_count].type = (OrderType)parse_uint64(&cursor); cursor++;
        buffer[parsed_count].price = (long)parse_uint64(&cursor); cursor++;
        buffer[parsed_count].qty = (uint32_t)parse_uint64(&cursor); cursor++;
        
        uint64_t ts = parse_uint64(&cursor); (void)ts;

        if (cursor < end && *cursor == '\r') cursor++;
        if (cursor < end && *cursor == '\n') cursor++;

        parsed_count++;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    stats->csv_load_time = TIME_DIFF(t0, t1);

    // ==========================================
    // PHASE 2 : INJECTION DANS LE MOTEUR
    // ==========================================
    clock_gettime(CLOCK_MONOTONIC, &t2);

    for (uint32_t i = 0; i < parsed_count; i++) {
        if (verbose && (i % 100000 == 0)) {
            printf("[Verbose] Traitement de l'ordre %u / %u...\n", i, parsed_count);
        }
        orderbook_add_order(book, buffer[i].id, buffer[i].type, buffer[i].side, buffer[i].price, buffer[i].qty);
    }

    clock_gettime(CLOCK_MONOTONIC, &t3);
    stats->matching_time = TIME_DIFF(t2, t3);
    
    // Remplir les stats finales
    stats->total_time = stats->csv_load_time + stats->matching_time;
    stats->total_orders = parsed_count;
    stats->total_trades = book->trade_count;

    // Nettoyage
    free(buffer);
    munmap(data, sb.st_size);
    close(fd);
}