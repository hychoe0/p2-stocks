// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#include <iostream>
#include "market.h"

using namespace std;

int main() {
  Market market;

  market.readFileHeader();

  cout << market.mode << endl;
  cout << market.num_traders << endl;
  cout << market.num_stocks << endl;
}