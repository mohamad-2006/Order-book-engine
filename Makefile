CC = gcc
# On a bien le -O3 et le -pg pour la compilation
CFLAGS = -Wall -Wextra -O3 -pg -march=native -Iinclude
# LDFLAGS est crucial pour que gprof fonctionne (l'étape du "liage")
LDFLAGS = -pg

SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests

# 1. Détection automatique de tous les fichiers d'en-tête (.h)
HEADERS = $(wildcard include/*.h)

# 2. Fichiers de logique (le cœur du moteur)
CORE_SRCS = $(SRC_DIR)/orderbook.c $(SRC_DIR)/order.c $(SRC_DIR)/rbtree.c $(SRC_DIR)/hashtable.c $(SRC_DIR)/mempool.c $(SRC_DIR)/parser.c
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 3. Cibles finales
TARGET = trade_engine
TEST_TARGET = test_orderbook
TEST_LOGIC_TARGET = test_logic

all: $(TARGET)

# --- Compilation de l'application principale ---
$(TARGET): $(CORE_OBJS) $(OBJ_DIR)/main.o
	$(CC) $(LDFLAGS) $^ -o $@

# --- Compilation du Benchmark de Performance (1M d'ordres) ---
test: $(CORE_OBJS) $(OBJ_DIR)/test_orderbook.o
	$(CC) $(LDFLAGS) $^ -o $(TEST_TARGET)

# --- Compilation des Tests Unitaires (Nouveau !) ---
test_logic: $(CORE_OBJS) $(OBJ_DIR)/test_logic.o
	$(CC) $(LDFLAGS) $^ -o $(TEST_LOGIC_TARGET)

# --- Règles de compilation génériques ---

# Compile les fichiers de src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile les fichiers de tests/ (s'applique à test_orderbook.c ET test_logic.c)
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier obj/ s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage complet
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET) $(TEST_LOGIC_TARGET) gmon.out diff_result.txt trades_out.csv

.PHONY: all clean test test_logic