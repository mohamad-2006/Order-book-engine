# Order-book-engine


# C-Trade Engine Architecture

This matching engine uses a high-performance data structure combining **Red-Black Trees** (for price-time priority ordering) and a **Hash Table** (for O(1) order cancellations). Prices are stored as integers (`int64_t`) to prevent floating-point precision loss.
![C](https://img.shields.io/badge/Language-C-blue.svg)
![Performance](https://img.shields.io/badge/Throughput-4.4M%20Orders%2Fsec-success.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

## 📊 Performance Benchmark

The engine is highly optimized, utilizing custom memory pools, `-O3` compiler optimizations, and efficient branch prediction. It completely avoids floating-point arithmetic (using `int64_t` for prices) to prevent precision loss and maximize CPU instruction speed.

**Hardware:** 11th Gen Intel(R) Core(TM) i7-11800H @ 2.30GHz  
**Dataset:** 1,000,000 randomized LIMIT orders

| Metric | Result |
| :--- | :--- |
| **Orders Processed** | 1,000,000 |
| **Trades Generated** | 547,442 |
| **CSV Load Time** | 0.214 seconds |
| **Pure Matching Time** | **0.223 seconds** |
| **Total Execution Time**| 0.438 seconds |
| **Throughput** | 🔥 **4,469,245 orders/sec** |

## System Architecture

```text
                     [ ORDER BOOK ]
                           │
      ┌────────────────────┼────────────────────┐
      │                    │                    │
[ BIDS (Buy) ]       [ ASKS (Sell) ]      [ ORDER MAP ]
 Red-Black Tree       Red-Black Tree       Hash Table (by ID)
      │                    │                    │
      ▼                    ▼                    ▼
(PriceLevel 100)     (PriceLevel 105)      [ ID: 45 ] ──┐
  total_qty: 50        total_qty: 20       [ ID: 89 ]   │ (O(1) Lookup)
      │                    │                            │
      ├─► Order(id:12)     ├─► Order(id:45) ◄───────────┘
      │     qty: 30        │     qty: 20
      │                    │
      └─► Order(id:89)     └─► NULL
            qty: 20
Overview

This project is a high-performance Order Book Matching Engine written in C.
It simulates the core mechanism used in financial markets to match buy and sell orders.

The engine processes incoming orders, maintains a live order book, and executes trades based on price-time priority, just like real-world exchanges.

Key Concepts

-   This project is based on fundamental concepts of market microstructure:

-   Order Book: Collection of buy (bid) and sell (ask) orders
-   Matching Engine: Matches orders automatically
-   Spread: Difference between best bid and best ask
-   Liquidity: Available volume at different price levels
-   Price-Time Priority:
    -    Best price gets priority
    -   Earlier orders at the same price are executed first


Features
Core Features:
-   Limit order handling (Buy / Sell)
-   Real-time order matching
-   Price-time priority execution
-   Partial order fills
-   Order book display
-   Market orders
-   Order cancellation
-   Unique order IDs
-   Timestamp tracking
-   Trade history logging


Project Structure
order-book/
│
├── src/
│   ├── dll.c
│   ├── hashtable.c
│   ├── main.c
│   ├── mempool.c
│   ├── order.c
│   ├── orderbook.c
│   ├── parser.c
│   └── rbtree.c
│
├── include/
│   ├── errors.h
│   ├── hastable.h
│   ├── mempool.h
│   ├── rbtree.h
│   ├── trade.h
│   └── types.h
│
├── tests/
│   ├── run_test.sh
│   ├── test_bst.c
│   ├── test_dll.c
│   ├── test_hashtable.c
│   ├── test_logic.c
│   ├── test_mempool.c
│   └── test_orderbook.c
│
├── .gitignore
├── generate_orders.py
├── Makefile
├── orders_1M.csv
└── README.md



Matching Logic
Buy Order Execution

A buy order is matched if:

buy_price >= best_ask_price
Sell Order Execution

A sell order is matched if:

sell_price <= best_bid_price
Execution Rules
Trades occur at the best available price
Orders can be:
Fully filled
Partially filled
Remaining quantity stays in the book
















Why This Project Matters

This is the core system behind financial exchanges like:

Stock markets
Crypto exchanges
Futures markets

Building this project demonstrates strong skills in:

Systems programming
Algorithm design
Financial engineering

Contributing

Contributions are welcome!

You can:

Improve performance
Add new features
Fix bugs
Refactor code

License

This project is open-source and available under the MIT License.

Author

Mohamadou

This project is a stepping stone toward quant engineering, HFT systems, and advanced trading infrastructure.
