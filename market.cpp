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

