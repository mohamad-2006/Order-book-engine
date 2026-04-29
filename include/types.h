#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

// Type d'ordre : LIMIT (prix fixe) ou MARKET (prix du marché)
typedef enum { LIMIT, MARKET } OrderType;

// Côté de l'ordre : ACHAT ou VENTE
typedef enum { BUY, SELL } OrderSide;
typedef uint64_t TIMESTAMP;
typedef int64_t Price;
#endif