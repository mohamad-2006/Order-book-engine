#include "orderbook.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*void test_matching() {
    printf("--- DEMARRAGE DU MOTEUR DE MATCHING ---\n\n");
    OrderBook* book = orderbook_create(1000);

    // 1. On place des vendeurs patients (Makers)
    printf("[1] Ajout d'un Vendeur à 15100 (Qté: 10)\n");
    orderbook_add_order(book, 1, SELL, 15100, 10);
    
    printf("[2] Ajout d'un Vendeur à 15000 (Qté: 5) -> Le meilleur prix !\n");
    orderbook_add_order(book, 2, SELL, 15000, 5); 

    printf("\n[3] Arrivee d'un Acheteur agressif (Taker) prêt à payer 15050 pour 10 parts...\n");
    // Il va matcher avec le Vendeur 2 (car 15000 < 15050), mais il restera bloqué pour le Vendeur 1 (15100 > 15050)
    orderbook_add_order(book, 3, BUY, 15050, 10);

    // 4. Vérifications mathématiques
    Order* acheteur = hashtable_get(book->order_map, 3);
    assert(acheteur != NULL);
    
    // L'acheteur voulait 10 parts. Il en a trouvé 5 à bon prix. Il doit lui en rester 5 en attente.
    assert(acheteur->quantity == 5); 
    
    // Le vendeur 2 a été totalement dévoré, il ne doit plus exister !
    assert(hashtable_get(book->order_map, 2) == NULL);

    printf("\nSUCCES : Le matching fonctionne, l'assert mathematique est valide !\n");
    
    free(book);
}*/
void load_and_match_csv(OrderBook* book, const char* filepath);

int main() {
    // On alloue un grand pool de 1 million d'ordres pour éviter les pannes mémoire
    OrderBook* book = orderbook_create(1000000); 

    printf("🚀 Lancement du test sur 1 Million d'ordres...\n");
    load_and_match_csv(book, "orders_1M.csv");

    // Si tu as gardé tes printf dans match_order, tu devras peut-être les 
    // commenter pour ce test, car faire 500 000 printf() dans le terminal va 
    // ralentir ton C (l'I/O console est très lente !).
    return 0;
}