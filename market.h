// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef MARKET_H
#define MARKET_H

// #include <string>
#include <iostream>
// #include <vector>
#include <queue>
#include <getopt.h>
#include "P2random.h"

using namespace std;

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

class Stock {
  // need two priority queue for buying and selling stock
  // priority_queue
};

class Market {
public:

  // Reading input file header (first 4 lines)
  void readFileHeader();

  // Setting argument for valid mode
  void getMode(int argc, char** argv);

  // receive a series of “orders”
  void getOrders();


  // MOVE TO PRIVATE AFTER TEST
  int num_traders;

  int num_stocks;

  string mode;

  bool verbose = false;

  bool median = false;

  bool trader_info = false;

  bool time_travelers = false;

  priority_queue<Orders, vector<Orders>, BuyComparator> buyingOrders;
  priority_queue<Orders, vector<Orders>, SellComparator> sellingOrders;

private:


  

};

#endif // MARKET_H