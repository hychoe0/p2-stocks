// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef MARKET_H
#define MARKET_H

// #include <string>
#include <iostream>
// #include <vector>
#include <queue>
#include <getopt.h>
#include "order.h"
#include "P2random.h"

using namespace std;

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