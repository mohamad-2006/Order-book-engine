# Nom de l'exécutable final
TARGET = bin/order_book

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = 

# Dossiers
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Lister tous les fichiers .c dans src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Transformer la liste des .c en liste de .o dans le dossier obj/
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# --- CIBLES ---

# Cible par défaut : construit l'exécutable
all: directories $(TARGET)

# Créer les dossiers bin et obj s'ils n'existent pas
directories:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)

# Lier les fichiers objets pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compiler chaque fichier .c en fichier .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour nettoyer le projet (supprimer bin/ et obj/)
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Cible pour exécuter le programme (test simple)
test: all
	./$(TARGET)

.PHONY: all clean test directories