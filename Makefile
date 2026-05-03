CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -O0
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests

# 1. On définit les fichiers de logique (tout sauf les fichiers avec un main)
# On exclut explicitement main.c pour la bibliothèque de logique
CORE_SRCS = $(SRC_DIR)/orderbook.c $(SRC_DIR)/order.c $(SRC_DIR)/rbtree.c $(SRC_DIR)/hashtable.c
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 2. Cibles finales
TARGET = trade_engine
TEST_TARGET = test_hashtable

all: $(TARGET)

# --- Compilation de l'application principale ---
$(TARGET): $(CORE_OBJS) $(OBJ_DIR)/main.o
	$(CC) $^ -o $@

# --- Compilation des tests ---
test: $(CORE_OBJS) $(OBJ_DIR)/test_hashtable.o
	$(CC) $^ -o $(TEST_TARGET)
	./$(TEST_TARGET)

# --- Règles de compilation génériques ---

# Compile les fichiers du dossier src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile les fichiers du dossier tests/
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test