# Order-book-engine

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

Additional Features (Optional / Advanced)
-   Market orders
-   Order cancellation
-   Unique order IDs
-   Timestamp tracking
-   Trade history logging


Project Structure
order-book/
│
├── src/
│   ├── main.c
│   ├── orderbook.c
│   ├── matching.c
│   └── utils.c
│
├── include/
│   ├── orderbook.h
│   ├── matching.h
│   └── utils.h
│
├── tests/
│   └── test_orders.c
│
├── Makefile
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