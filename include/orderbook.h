#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <time.h>

// Type d'ordre : LIMIT (prix fixe) ou MARKET (prix du marché)
typedef enum { LIMIT, MARKET } OrderType;

// Côté de l'ordre : ACHAT ou VENTE
typedef enum { BUY, SELL } OrderSide;

typedef struct Order {
    int id;
    OrderType type;
    OrderSide side;
    double price;
    int quantity;
    time_t timestamp;      // Stocke l'heure de création
    struct Order* next;    // Pour la future liste chaînée
} Order;

// Prototypes des fonctions
Order* create_order(int id, OrderType type, OrderSide side, double price, int quantity);
void free_order(Order* order);

#endif