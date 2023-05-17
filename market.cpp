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