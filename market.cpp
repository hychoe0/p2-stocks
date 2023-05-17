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
  int count = 0;

  if (mode == "TL") {
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

                order.placement = count;
                ++count;

               } // while ... cin each order
  } // if ... TL mode
} // getOrders()