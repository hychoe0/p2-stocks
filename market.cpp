// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#include "market.h"
#include <iostream>
#include <string>
#include <algorithm>

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

  // Initialize stockList to num_stocks
  stockList.resize(static_cast<size_t>(num_stocks));
  // Initialize stockList to num_traders
  traderList.resize(static_cast<size_t>(num_traders));

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
  total_trade = 0;

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

      if (order.intent == 'B') {
        stockList[static_cast<size_t>(order.stock_id)].
        buyingOrders.push(order);
      }
      else if (order.intent == 'S') {
        stockList[static_cast<size_t>(order.stock_id)].
        sellingOrders.push(order);
      }
      else {
        cerr << "Error: Invalid order intent" << endl;
        exit(1);
      }

      // This means the timestamp of the order changed, time for median
      if (curr_timestamp != order.timestamp && median) {
        for (size_t stockID = 0;
             stockID < static_cast<size_t>(num_stocks);
             ++stockID) {
          int medianValue = calculateMedian(stockList[stockID].matchOrders);
          if (medianValue >= 0) {
            cout << "Median match price of Stock " << stockID << " at time "
                 << curr_timestamp << " is $" << medianValue << "\n";
          }
        }
      }
      // decreasing timestamp
      curr_timestamp = order.timestamp; 

      // TODO: processing time traveler mode and median mode
      trade();
      } // while ... cin each order

      if (median) {
        median_timestamp = order.timestamp;
      }
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

  // The very last timestamp median value
  if (median) {
    for (size_t stockID = 0;
         stockID < static_cast<size_t>(num_stocks);
         ++stockID) {
      int medianValue = calculateMedian(stockList[stockID].matchOrders);
      if (medianValue >= 0) {
        cout << "Median match price of Stock " << stockID << " at time "
              << median_timestamp << " is $" << medianValue << "\n";
      }
    }
  } // if ... very last median

  // End of Day...
  printResult();

  if (trader_info) {
    cout << "---Trader Info---\n";
    for (size_t id = 0; id < static_cast<size_t>(num_traders); ++id) {
      cout << "Trader " << id << " bought " << traderList[id].purchase
           << " and sold " << traderList[id].sale << " for a net transfer of "
           << "$" << traderList[id].net << "\n";
    }
  }

} // getOrders()

void Market::processOrders(istream &inputStream) {
  Orders order;

  string junk;
  char chunk;
  int count = 0;
  int curr_timestamp = 0;

  // Read orders from inputStream, NOT cin
  while (inputStream >> order.timestamp >> order.intent >> junk >> chunk
                     >> order.trader_id >> chunk >> order.stock_id >> chunk
                     >> order.price >> chunk >> order.quantity) {

    // // to output PR in TL mode      
    // string b_s;
    // if (order.intent == 'B') {
    //   b_s = "BUY";
    // }
    // else {
    //   b_s = "SELL";
    // }
    // cout << order.timestamp << " " << b_s << " T" << order.trader_id
    //       << " S" << order.stock_id << " $" << order.price << " #"
    //       << order.quantity << endl;

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

    // This means the timestamp of the order changed, time for median
    if (curr_timestamp != order.timestamp && median) {
      for (size_t stockID = 0;
           stockID < static_cast<size_t>(num_stocks);
           ++stockID) {
        int medianValue = calculateMedian(stockList[stockID].matchOrders);
        if (medianValue >= 0) {
          cout << "Median match price of Stock " << stockID << " at time "
                << curr_timestamp << " is $" << medianValue << "\n";
        }
      }
    }
    // decreasing timestamp
    curr_timestamp = order.timestamp; 

    // TODO: processing time traveler mode and median mode
    trade();

  }  // while ..inputStream
  
  if (median) {
    median_timestamp = order.timestamp;
  }

} // processOrders()

void Market::trade() {
  for (size_t stockID = 0;
       stockID < static_cast<size_t>(num_stocks);
       ++stockID) {

    while (!stockList[stockID].buyingOrders.empty() &&
           !stockList[stockID].sellingOrders.empty()) {
      // Orders topBuyOrder = stockList[stockID].buyingOrders.top();
      // Orders topSellOrder = stockList[stockID].sellingOrders.top();

      // if selling price is higher than buying price, trade doesn't work
      if (stockList[stockID].buyingOrders.top().price <
          stockList[stockID].sellingOrders.top().price) {
        break;
      }

      // trade happens (successful trade)
      else {
        // When buyQ > sellQ, buyQ - sellQ
        if (stockList[stockID].buyingOrders.top().quantity >
            stockList[stockID].sellingOrders.top().quantity) {

          // if the BUY order arrived first,
          // then the price of the match will be the buyer's price
          if (stockList[stockID].buyingOrders.top().placement >
              stockList[stockID].sellingOrders.top().placement) {
            if (verbose) {
              printVerbose(stockList[stockID].buyingOrders.top().trader_id,
                          stockList[stockID].sellingOrders.top().quantity,
                          static_cast<int>(stockID),
                          stockList[stockID].sellingOrders.top().trader_id,
                          stockList[stockID].sellingOrders.top().price);
            } // if ... verbose

            if (median) {
              stockList[stockID].matchOrders.push_back
              (stockList[stockID].sellingOrders.top().price);
            } // if .. median

            if (trader_info) {
              size_t buy_id =
              static_cast<size_t>(stockList[stockID].buyingOrders.top().trader_id);
              
              size_t sell_id =
              static_cast<size_t>(stockList[stockID].sellingOrders.top().trader_id);

              traderList[buy_id].purchase +=
              stockList[stockID].sellingOrders.top().quantity;
              
              traderList[sell_id].sale +=
              stockList[stockID].sellingOrders.top().quantity;
              
              traderList[sell_id].net +=
              stockList[stockID].sellingOrders.top().quantity * 
              stockList[stockID].sellingOrders.top().price;

              traderList[buy_id].net -=
              stockList[stockID].sellingOrders.top().quantity * 
              stockList[stockID].sellingOrders.top().price;
            } // if ... trader_info

          } // if ... sellingOrder came first

          else {
            if (verbose) {
              printVerbose(stockList[stockID].buyingOrders.top().trader_id,
                          stockList[stockID].sellingOrders.top().quantity,
                          static_cast<int>(stockID),
                          stockList[stockID].sellingOrders.top().trader_id,
                          stockList[stockID].buyingOrders.top().price);
            } // if ... verbose

            if (median) {
              stockList[stockID].matchOrders.push_back
              (stockList[stockID].buyingOrders.top().price);
            } // if .. median

            if (trader_info) {
              size_t buy_id =
              static_cast<size_t>(stockList[stockID].buyingOrders.top().trader_id);
              
              size_t sell_id =
              static_cast<size_t>(stockList[stockID].sellingOrders.top().trader_id);

              traderList[buy_id].purchase +=
              stockList[stockID].sellingOrders.top().quantity;
              
              traderList[sell_id].sale +=
              stockList[stockID].sellingOrders.top().quantity;
              
              traderList[sell_id].net +=
              stockList[stockID].sellingOrders.top().quantity * 
              stockList[stockID].buyingOrders.top().price;

              traderList[buy_id].net -=
              stockList[stockID].sellingOrders.top().quantity * 
              stockList[stockID].buyingOrders.top().price;
            } // if ... trader_info

          } // else ... buyingOrder came first

          Orders modifiedStock = stockList[stockID].buyingOrders.top();

          modifiedStock.quantity -=
          stockList[stockID].sellingOrders.top().quantity;

          stockList[stockID].sellingOrders.pop();
          stockList[stockID].buyingOrders.pop();
          stockList[stockID].buyingOrders.push(modifiedStock);
          ++total_trade;
          
        } // if ... buyingOrder.quantity > sellingOrder.quantity

        // When sellQ > buyQ, sellQ - buyQ
        else if (stockList[stockID].buyingOrders.top().quantity <
                stockList[stockID].sellingOrders.top().quantity) {
          
          // if the BUY order arrived first,
          // then the price of the match will be the buyer's price
          if (stockList[stockID].buyingOrders.top().placement >
              stockList[stockID].sellingOrders.top().placement) {
            if (verbose) {
              printVerbose(stockList[stockID].buyingOrders.top().trader_id,
                          stockList[stockID].buyingOrders.top().quantity,
                          static_cast<int>(stockID),
                          stockList[stockID].sellingOrders.top().trader_id,
                          stockList[stockID].sellingOrders.top().price);
            } // if ... verbose

            if (median) {
              stockList[stockID].matchOrders.push_back
              (stockList[stockID].sellingOrders.top().price);
            } // if .. median

            if (trader_info) {
              size_t buy_id =
              static_cast<size_t>(stockList[stockID].buyingOrders.top().trader_id);
              
              size_t sell_id =
              static_cast<size_t>(stockList[stockID].sellingOrders.top().trader_id);

              traderList[buy_id].purchase +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].sale +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].net +=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].sellingOrders.top().price;

              traderList[buy_id].net -=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].sellingOrders.top().price;
            } // if ... trader_info

          } // if ... sellingOrder came first
          else {
            if (verbose) {
              printVerbose(stockList[stockID].buyingOrders.top().trader_id,
                          stockList[stockID].buyingOrders.top().quantity,
                          static_cast<int>(stockID),
                          stockList[stockID].sellingOrders.top().trader_id,
                          stockList[stockID].buyingOrders.top().price);
            } // if ... verbose

            if (median) {
              stockList[stockID].matchOrders.push_back
              (stockList[stockID].buyingOrders.top().price);
            } // if .. median

            if (trader_info) {
              size_t buy_id =
              static_cast<size_t>(stockList[stockID].buyingOrders.top().trader_id);
              
              size_t sell_id =
              static_cast<size_t>(stockList[stockID].sellingOrders.top().trader_id);

              traderList[buy_id].purchase +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].sale +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].net +=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].buyingOrders.top().price;

              traderList[buy_id].net -=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].buyingOrders.top().price;
            } // if ... trader_info

          } // else ... buyingOrder came first

          Orders modifiedStock = stockList[stockID].sellingOrders.top();

          modifiedStock.quantity -=
          stockList[stockID].buyingOrders.top().quantity;

          stockList[stockID].sellingOrders.pop();
          stockList[stockID].buyingOrders.pop();
          stockList[stockID].sellingOrders.push(modifiedStock);
          ++total_trade;

        } // if ... buyingOrder.quantity < sellingOrder.quantity

        else {
          if (stockList[stockID].buyingOrders.top().placement >
              stockList[stockID].sellingOrders.top().placement) {
            if (verbose) {
              printVerbose(stockList[stockID].buyingOrders.top().trader_id,
                          stockList[stockID].buyingOrders.top().quantity,
                          static_cast<int>(stockID),
                          stockList[stockID].sellingOrders.top().trader_id,
                          stockList[stockID].sellingOrders.top().price);
            } // if ... verbose

            if (median) {
              stockList[stockID].matchOrders.push_back
              (stockList[stockID].sellingOrders.top().price);
            } // if .. median

            if (trader_info) {
              size_t buy_id =
              static_cast<size_t>(stockList[stockID].buyingOrders.top().trader_id);
              
              size_t sell_id =
              static_cast<size_t>(stockList[stockID].sellingOrders.top().trader_id);

              traderList[buy_id].purchase +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].sale +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].net +=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].sellingOrders.top().price;

              traderList[buy_id].net -=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].sellingOrders.top().price;
            } // if ... trader_info

          } // if ... sellingOrder came first
          else {
            if (verbose) {
              printVerbose(stockList[stockID].buyingOrders.top().trader_id,
                          stockList[stockID].buyingOrders.top().quantity,
                          static_cast<int>(stockID),
                          stockList[stockID].sellingOrders.top().trader_id,
                          stockList[stockID].buyingOrders.top().price);
            } // if ... verbose

            if (median) {
              stockList[stockID].matchOrders.push_back
              (stockList[stockID].buyingOrders.top().price);
            } // if .. median

            if (trader_info) {
              size_t buy_id =
              static_cast<size_t>(stockList[stockID].buyingOrders.top().trader_id);
              
              size_t sell_id =
              static_cast<size_t>(stockList[stockID].sellingOrders.top().trader_id);

              traderList[buy_id].purchase +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].sale +=
              stockList[stockID].buyingOrders.top().quantity;
              
              traderList[sell_id].net +=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].buyingOrders.top().price;

              traderList[buy_id].net -=
              stockList[stockID].buyingOrders.top().quantity * 
              stockList[stockID].buyingOrders.top().price;
            } // if ... trader_info

          } // else ... buyingOrder came first

          // When both have same quantity, both pop from pq ("complete trade")
          stockList[stockID].buyingOrders.pop();
          stockList[stockID].sellingOrders.pop();
          ++total_trade;

        } // else ... buyingOrder.quantity == sellingOrder.quantity

      } // else ... when successful trade happens
              
    } // while ... stockList[stockID].buy and sell.empty()
  } // for ... stockID < num_stocks
} // trade()

void Market::printVerbose(int buyerID, int sellQuantity, int stockID,
                          int sellerID, int soldPrice) {
  
  cout << "Trader " << buyerID << " purchased " << sellQuantity
       << " shares of Stock " << stockID << " from Trader "
       << sellerID << " for $" << soldPrice << "/share\n";

}

void Market::printResult() {
  cout << "---End of Day---\n" << "Trades Completed: " << total_trade << "\n";
}

int Market::calculateMedian(vector<int> &orders) {
  if (orders.empty()) {
    return -1;
  }

  sort(orders.begin(), orders.end()); // Sort in ascending Order

  if (orders.size() % 2 == 0) {
    int middle1 = orders[(orders.size() / 2) - 1];
    int middle2 = orders[orders.size() / 2];
    return (middle1 + middle2) / 2;
  } // if ... order is EVEN

  else {
    return orders[orders.size() / 2];
  } // else ... order is ODD

}