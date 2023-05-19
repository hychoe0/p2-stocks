// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#include <iostream>
#include "market.h"
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  Market market;

  market.readFileHeader();
  market.getMode(argc, argv);

  cout << "Processing orders...\n";
  
  market.getOrders();

  

  // // TEST
  // for (int i = 0; i < market.num_stocks; ++i) {
  //   Orders topBuyingOrder = market.stockList[static_cast<size_t>(i)].buyingOrders.top();
  //   Orders topSellingOrder = market.stockList[static_cast<size_t>(i)].sellingOrders.top();
  //   // market.stockList[static_cast<size_t>(i)].sellingOrders.pop();
  //   // topSellingOrder = market.stockList[static_cast<size_t>(i)].sellingOrders.top();

  //   cout << "Top Buying Order for Stock "
  //        << topBuyingOrder.stock_id << ": " << topBuyingOrder.timestamp << " "
  //        << topBuyingOrder.intent << " " << topBuyingOrder.trader_id << " "
  //        << topBuyingOrder.stock_id << " " << topBuyingOrder.price << " "
  //        << topBuyingOrder.quantity << " " << topBuyingOrder.placement << endl;

  //   cout << "Top Selling Order for Stock "
  //        << topSellingOrder.stock_id << ": " << topSellingOrder.timestamp << " "
  //        << topSellingOrder.intent << " " << topSellingOrder.trader_id << " "
  //        << topSellingOrder.stock_id << " " << topSellingOrder.price << " "
  //        << topSellingOrder.quantity << " " << topSellingOrder.placement << endl;
  // }
  

  // cout << market.mode << endl;
  // cout << market.num_traders << endl;
  // cout << market.num_stocks << endl;
  // cout << "v: " << market.verbose << endl;
  // cout << "m: " << market.median << endl;
  // cout << "i: " << market.trader_info << endl;
  // cout << "t: " << market.time_travelers << endl;
}