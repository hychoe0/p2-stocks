// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#include <iostream>
#include "market.h"
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  Market market;

  market.readFileHeader();
  market.getMode(argc, argv);
  market.getOrders();

  cout << "Processing orders...\n";

  // if (market.stockListbuyingOrders.empty()) {
  //   cout << "No Orders in buying order" << endl;
  // }
  // else {
  //   Orders topOrder = market.sellingOrders.top();
  //   cout << "Top Order: " << topOrder.timestamp << " "
  //        << topOrder.intent << " " << topOrder.trader_id << " "
  //        << topOrder.stock_id << " " << topOrder.price << " "
  //        << topOrder.quantity << " " << topOrder.placement << endl;
  // }

  // cout << market.mode << endl;
  // cout << market.num_traders << endl;
  // cout << market.num_stocks << endl;
  // cout << "v: " << market.verbose << endl;
  // cout << "m: " << market.median << endl;
  // cout << "i: " << market.trader_info << endl;
  // cout << "t: " << market.time_travelers << endl;
}