// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef MARKET_H
#define MARKET_H

// #include <string>
#include <iostream>
// #include <vector>
#include <queue>
#include <getopt.h>
#include <algorithm>
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

  Orders()
        : timestamp(0), intent('\0'), trader_id(0), stock_id(0), price(0), quantity(0) {}

  Orders(int ts, char i, int tId, int sId, int p, int q)
        : timestamp(ts), intent(i), trader_id(tId), stock_id(sId), price(p), quantity(q) {}
};

struct BuyComparator {
  bool operator()(const Orders& o1, const Orders& o2) {
    if (o1.price == o2.price) {
      return o1.placement > o2.placement;
    }
    return o1.price < o2.price;
  }
};

struct SellComparator {
    bool operator()(const Orders& o1, const Orders& o2) {
      if (o1.price == o2.price) {
        return o1.placement > o2.placement;
      }
      return o1.price > o2.price;
    }
};

struct Stocks {
  // need two priority queue for buying and selling stock
  priority_queue<Orders, vector<Orders>, BuyComparator> buyingOrders;
  priority_queue<Orders, vector<Orders>, SellComparator> sellingOrders;

  // vector to store successful order prices
  vector<int> matchOrders;
};

struct Trader {
  int purchase = 0;
  int sale = 0;
  int net = 0;
};

class Market {
public:

  // Reading input file header (first 4 lines)
  void readFileHeader();

  // Setting argument for valid mode
  void getMode(int argc, char** argv);

  // Receive a series of “orders”
  void getOrders();

  // TODO: Create a separate function as referenced above, accepting a stream
  //       reference variable, to which you will pass cin or a stringstream
  //       that is populated by PR_init()
  void processOrders(istream &inputStream);

  void completeTrade(vector<Stocks> &stockList, size_t stockID);

  // Executing trade
  void trade();

  void printVerbose(int buyerID, int sellQuantity, int stockID, 
                    int sellerID, int soldPrice);

  void printResult();

  int calculateMedian(vector<int> &orders);


  // MOVE TO PRIVATE AFTER TEST
  int num_traders;

  int num_stocks;

  string mode;

  bool verbose = false;

  bool median = false;

  bool trader_info = false;

  bool time_travelers = false;

  vector<Orders> timeTraveler;
  vector<Stocks> stockList;
  vector<Trader> traderList;

  int total_trade;

  int median_timestamp;

private:


  

};

#endif // MARKET_H