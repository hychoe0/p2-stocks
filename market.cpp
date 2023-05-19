// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#include "market.h"
#include <iostream>
#include <string>

using namespace std;

void Market::readFileHeader() {
  string junk;
  
  getline(cin, junk); // omit COMMENT line
  cin >> junk; // "MODE: "
  
  cin >> mode;

  cin >> junk; // "NUM_TRADERS: "

  cin >> num_traders;

  cin >> junk; // "NUM_STOCKS: "

  cin >> num_stocks;

  // Resize stockList to num_stocks
  stockList.resize(static_cast<size_t>(num_stocks));

  // //TEST
  // for (size_t i = 0; i < stockList.size(); ++i) {
  //   if (stockList[i].buyingOrders.empty() &&
  //       stockList[i].sellingOrders.empty()) {
  //     cout << "empty" << endl;
  //   }
  // }
}

void Market::getMode(int argc, char** argv) {
  opterr = false;
  int choice;
  int index = 0;
  option long_options[] = {
    { "verbose", no_argument, nullptr, 'v' },
    { "median", no_argument, nullptr, 'm' },
    { "trader_info", no_argument, nullptr, 'i' },
    { "time_travelers", no_argument, nullptr, 't' },
    { nullptr, 0, nullptr, '\0' }
  };

  while ((choice = getopt_long
                   (argc, argv, "vmit", long_options, &index)) != -1) {
    switch (choice) {

      // Verbose
      case 'v':
        verbose = true;
        break;

      // median
      case 'm':
        median = true;
        break;

      // trader_info
      case 'i':
        trader_info = true;
        break;

      // time_travelers
      case 't':
        time_travelers = true;
        break;

      // Other Options are unknown
      default:
        cerr << "Error: Unknown command line option" << endl;
        cerr << "Default Error Found" << endl;
        exit(1);
    }  // switch ..choice
  }  // while
}

void Market::getOrders() {
  Orders order;

  string junk;
  char chunk;

  if (mode == "TL") {

    int count = 0;
    int curr_timestamp = 0;

    while (cin >> order.timestamp
               >> order.intent >> junk // BUY or SELL
               >> chunk >> order.trader_id // T_
               >> chunk >> order.stock_id // S_
               >> chunk >> order.price // $_
               >> chunk >> order.quantity // #_
               ) {
      // Error handling
      if (order.timestamp < 0) {
        cerr << "Error: Negative timestamp" << endl;
        exit(1);
      }

      if (order.trader_id < 0 || order.trader_id >= num_traders) {
        cerr << "Error: Invalid trader ID" << endl;
        exit(1);
      }

      if (order.stock_id < 0 || order.stock_id >= num_stocks) {
        cerr << "Error: Invalid stock ID" << endl;
        exit(1);
      }

      if (order.price < 0) {
        cerr << "Error: Invalid price" << endl;
        exit(1);
      }

      if (order.quantity < 0) {
        cerr << "Error: Invalid quantity" << endl;
        exit(1);
      }

      if (curr_timestamp > order.timestamp) {
        cerr << "Error: Decreasing timestamp" << endl;
        exit(1);
      }

      order.placement = count;
      ++count;

      // decreasing timestamp
      curr_timestamp = order.timestamp; 

      if (order.intent == 'B') {
        stockList[static_cast<size_t>(order.stock_id)].buyingOrders.push(order);
      }
      else if (order.intent == 'S') {
        stockList[static_cast<size_t>(order.stock_id)].sellingOrders.push(order);
      }
      else {
        cerr << "Error: Invalid order intent" << endl;
        exit(1);
      }

      // TODO: processing time traveler mode and median mode
      trade();
      } // while ... cin each order
  } // if ... TL mode

  else if (mode == "PR") {
    int seed;
    int num_orders;
    int rate;

    cin >> junk >> seed >> junk >> num_orders >> junk >> rate;

    stringstream ss;

    P2random::PR_init(ss, static_cast<unsigned int>(seed),
                      static_cast<unsigned int>(num_traders), 
                      static_cast<unsigned int>(num_stocks),
                      static_cast<unsigned int>(num_orders),
                      static_cast<unsigned int>(rate));

    processOrders(ss);

  } // else if ... PR mode
} // getOrders()

void Market::processOrders(istream &inputStream) {
  Orders order;

  string junk;
  char chunk;
  int count = 0;
  // int curr_timestamp = 0;

  // Read orders from inputStream, NOT cin
  while (inputStream >> order.timestamp >> order.intent >> junk >> chunk
                     >> order.trader_id >> chunk >> order.stock_id >> chunk
                     >> order.price >> chunk >> order.quantity) {
    // process orders
    order.placement = count;
    ++count;

    // // Decreasing timestamp
    // curr_timestamp = order.timestamp;

    if (order.intent == 'B') {
      stockList[static_cast<size_t>(order.stock_id)].buyingOrders.push(order);
    }
    else if (order.intent == 'S') {
      stockList[static_cast<size_t>(order.stock_id)].sellingOrders.push(order);
    }
    else {
      cerr << "Error: Invalid order intent" << endl;
      exit(1);
    }

    // TODO: processing time traveler mode and median mode

  }  // while ..inputStream
} // processOrders()

// void Market::completeTrade(vector<Stocks> &stockList, size_t stockID) {
  
// } // completeTrade()

void Market::trade() {
  for (size_t stockID = 0;
       stockID < static_cast<size_t>(num_stocks);
       ++stockID) {

    while (!stockList[stockID].buyingOrders.empty() &&
           !stockList[stockID].sellingOrders.empty()) {
      Orders topBuyOrder = stockList[stockID].buyingOrders.top();
      Orders topSellOrder = stockList[stockID].sellingOrders.top();

      // if selling price is higher than buying price, trade doesn't work
      if (stockList[stockID].buyingOrders.top().price <
          stockList[stockID].sellingOrders.top().price) {
        break;
      }
      // else if (stockList[stockID].buyingOrders.top().price >=
      //          stockList[stockID].sellingOrders.top().price) {
      // trade happens
      else {
        if (stockList[stockID].buyingOrders.top().quantity >
            stockList[stockID].sellingOrders.top().quantity) {
          // When buyQ > sellQ, buyQ - sellQ and sellingOrder.pop()
          // successful trade
          topBuyOrder.quantity -= topSellOrder.quantity;
          stockList[stockID].sellingOrders.pop();
          stockList[stockID].buyingOrders.pop();
          stockList[stockID].buyingOrders.push(topBuyOrder);
          cout << "buyQ > sellQ" << endl;
          
        } // if ... buyingOrder.quantity > sellingOrder.quantity
        else if (stockList[stockID].buyingOrders.top().quantity <
                stockList[stockID].sellingOrders.top().quantity) {
          // When sellQ > buyQ, sellQ - buyQ and buyingOrder.pop()
          // successful trade
          topSellOrder.quantity -= topBuyOrder.quantity;
          stockList[stockID].sellingOrders.pop();
          stockList[stockID].buyingOrders.pop();
          stockList[stockID].buyingOrders.push(topSellOrder);
          cout << "sellQ > buyQ" << endl;

        } // if ... buyingOrder.quantity < sellingOrder.quantity
        else {
          // When both have same quantity, both pop from pq ("complete trade")
          stockList[stockID].buyingOrders.pop();
          stockList[stockID].buyingOrders.pop();
        } // else ... buyingOrder.quantity == sellingOrder.quantity
      } // else ... when trade happens
                // completed trade
                // if (verbose)
                // buyer purchased with selling price

    } // while ... stockList[stockID].buy and sell.empty()
  } // for ... stockID < num_stocks
} // trade()