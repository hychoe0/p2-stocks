// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <queue>

struct Orders {
  int timestamp;
  char intent; // buy or sell
  int trader_id;
  int stock_id;
  int price;
  int quantity;

  int placement; // determine each order's placement time
};

// Comparator for buying orders (higher price has higher priority)
struct BuyComparator {
  bool operator()(const Orders& o1, const Orders& o2) const {
    if (o1.price == o2.price) {
      return o1.placement > o2.placement;
    }
    return o1.price < o2.price;
  }
};

// Comparator for selling orders (lower price has higher priority)
struct SellComparator {
    bool operator()(const Orders& o1, const Orders& o2) const {
      if (o1.price == o2.price) {
        return o1.placement > o2.placement;
      }
      return o1.price > o2.price;
    }
};

#endif // ORDER_H