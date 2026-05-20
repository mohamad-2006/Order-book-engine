CC = gcc
CFLAGS = -Wall -Wextra -O3 -pg -march=native -Iinclude
LDFLAGS = -pg
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests

# 1. Détection automatique de tous les fichiers d'en-tête (.h)
# Ainsi, si un seul .h est modifié, 'make' saura qu'il faut recompiler les .o associés !
HEADERS = $(wildcard include/*.h)

# 2. On définit les fichiers de logique (Correction du doublon de orderbook.c)
CORE_SRCS = $(SRC_DIR)/orderbook.c $(SRC_DIR)/order.c $(SRC_DIR)/rbtree.c $(SRC_DIR)/hashtable.c $(SRC_DIR)/mempool.c $(SRC_DIR)/parser.c
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 3. Cibles finales
TARGET = trade_engine
TEST_TARGET = test_orderbook

all: $(TARGET)

# --- Compilation de l'application principale ---
$(TARGET): $(CORE_OBJS) $(OBJ_DIR)/main.o
	$(CC) $^ -o $@

# --- Compilation et exécution automatique des tests ---
test: $(CORE_OBJS) $(OBJ_DIR)/test_orderbook.o
	$(CC) $^ -o $(TEST_TARGET)
	./$(TEST_TARGET)
	$(OBJS)
	gcc -pg $(OBJS) -o $(EXEC) 

# --- Règles de compilation génériques ---

# Compile les fichiers de src/ en prenant en compte les changements dans include/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile les fichiers de tests/ en prenant en compte les changements dans include/
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création sécurisée du dossier obj/ s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage complet
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test