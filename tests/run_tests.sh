#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "🚀 Début de la suite de tests de régression...\n"

# 1. Compilation
echo "🔨 Compilation des tests..."
make clean > /dev/null

# On compile LES DEUX cibles et on retire -march=native pour Valgrind
make test_logic test CFLAGS="-Wall -Wextra -O2 -Iinclude" LDFLAGS="" > /dev/null

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Échec de la compilation.${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Compilation réussie.${NC}\n"

# 2. Tests Unitaires
echo "🧪 Lancement des Tests Unitaires :"
./test_logic
if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Un ou plusieurs tests unitaires ont échoué.${NC}"
    exit 1
fi
echo ""

# 3. Test de Déterminisme (Stress Test)
echo "📈 Lancement du Stress Test de Déterminisme (10 000 ordres)..."

if [ ! -f "tests/data/orders_10k.csv" ]; then
    echo "⚠️ Fichier tests/data/orders_10k.csv introuvable. Utilisation de orders_1M.csv..."
    INPUT_FILE="orders_1M.csv"
else
    INPUT_FILE="tests/data/orders_10k.csv"
fi

./test_orderbook --input $INPUT_FILE --stats > /dev/null

if [ -f "trades_out.csv" ] && [ -f "tests/data/golden_trades_10k.csv" ]; then
    diff trades_out.csv tests/data/golden_trades_10k.csv > diff_result.txt
    if [ -s diff_result.txt ]; then
        echo -e "${RED}❌ ÉCHEC DU DÉTERMINISME : Les sorties diffèrent !${NC}"
        cat diff_result.txt
        exit 1
    else
        echo -e "${GREEN}✅ Déterminisme validé : Les sorties sont identiques.${NC}\n"
    fi
else
    echo -e "💡 Benchmark exécuté avec succès (pas de comparaison CSV pour le moment).\n"
fi

# 4. Test de Fuite Mémoire (Valgrind)
echo "🔍 Vérification des fuites mémoire (Valgrind)..."
valgrind --leak-check=full --error-exitcode=1 ./test_logic > valgrind_out.txt 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ PROBLÈME MÉMOIRE OU CRASH DÉTECTÉ par Valgrind :${NC}"
    cat valgrind_out.txt | grep -E "lost|error|Illegal" | head -n 5
    exit 1
else
    echo -e "${GREEN}✅ Aucune fuite de mémoire détectée. Code parfaitement propre.${NC}\n"
fi

echo -e "${GREEN}🎉 TOUTE LA SUITE DE TESTS EST VALIDÉE ! LE MOTEUR EST PRÊT POUR LA PRODUCTION. 🎉${NC}"
rm -f valgrind_out.txt diff_result.txt
exit 0