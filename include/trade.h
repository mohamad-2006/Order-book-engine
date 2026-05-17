#ifndef TRADE_H
#define TRADE_H

#include "order.h"
#include <stdint.h>


// Structure pour archiver chaque execution
typedef struct Trade
{
    uint64_t buyer_id;  // ID de l'acheteur
    uint64_t seller_id; // ID du vendeur
    Price price;        // Prix d'execution
    uint32_t quantity;  // Quantité échangée
    TIMESTAMP timestamp;// Heure du trade
} Trade;

#endif