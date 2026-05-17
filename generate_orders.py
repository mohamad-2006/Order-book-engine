import csv
import random
import time

NUM_ORDERS = 1_000_000
FILENAME = "orders_1M.csv"

# Constantes pour coller à tes Enum C (0=BUY, 1=SELL / 0=LIMIT, 1=MARKET)
SIDE_BUY = 0
SIDE_SELL = 1
TYPE_LIMIT = 0
TYPE_MARKET = 1

print(f"Génération de {NUM_ORDERS} ordres en cours...")
start_time = time.time()

with open(FILENAME, mode='w', newline='') as file:
    writer = csv.writer(file)
    # L'en-tête que notre parser C va sauter
    writer.writerow(['order_id', 'side', 'type', 'price', 'quantity', 'timestamp'])
    
    for i in range(1, NUM_ORDERS + 1):
        order_id = i
        side = random.choice([SIDE_BUY, SIDE_SELL])
        
        # 95% d'ordres LIMIT, 5% d'ordres MARKET (agressifs)
        type_order = TYPE_LIMIT if random.random() < 0.95 else TYPE_MARKET
        
        # On simule un prix qui oscille autour de 15000 pour forcer le matching
        if side == SIDE_BUY:
            price = random.randint(14950, 15050)
        else:
            price = random.randint(14980, 15100)
            
        qty = random.randint(1, 100)
        ts = int(time.time() * 1000) + i # Timestamp simulé en ms
        
        writer.writerow([order_id, side, type_order, price, qty, ts])

print(f"Fichier '{FILENAME}' généré avec succès en {time.time() - start_time:.2f} secondes !")