// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef MARKET_H
#define MARKET_H

// #include <string>
#include <iostream>
#include <vector>
#include <getopt.h>
#include "P2random.h"

class Stock {

};

// class

class Market {
public:

  void readFileHeader();

  // Setting argument for valid mode
  void getMode(int argc, char** argv);

  // MOVE TO PRIVATE AFTER TEST
  int num_traders;

  int num_stocks;

  std::string mode;

  bool verbose = false;

  bool median = false;

  bool trader_info = false;

  bool time_travelers = false;

private:


  

};

#endif // MARKET_H