#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ENGINE_SUCCESS = 0,
    ERR_ORDER_NOT_FOUND = -1,
    ERR_INVALIDE_QUANTITY = -2,
    ERR_BOOK_EMPTY = -4,
    ERR_MEMORY_ALLOC = -5
} EngineStatus;

#endif