//
//  PerformanceTest.cpp
//  Test
//
//  Created by SiranWang on 3/22/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
#include "../../src/MarketEvents.h"
#include "../../src/MatchSimulation.h"
#include "../../src/MarketEvents.cpp"
using namespace Matching;
using namespace std;

int run_test(OrderBook& order_book, vector<vector<long>>& orders, clock_t end) {
  int count = 0, i = 0;
  do {
      Side side = orders[i][1]==0 ? Buy : Sell;
      order_book.Enter(orders[i][0], side, orders[i][2], orders[i][3]);
      //if((i%23)==0) order_book.Cancel(orders[i][0]);
      ++count,++i;
      if(i == orders.size()-1) break;
  } while (clock() < end);
  return count;
}

bool build_and_run_test(uint32_t dur_sec, uint32_t num_to_try) {
  std::cout << "trying run of " << num_to_try << " orders\n";
    MarketEvents* me = new MarketEvents();
    OrderBook order_book = OrderBook(me);
    vector<vector<long>> orders;
  
  for (uint32_t i = 0; i <= num_to_try; ++i) {
    long side = ((i % 2)== 0 ? 0 : 1);
    uint32_t delta = side ? 1880 : 1884;
    // ASK 1893
    // ASK 1892
    // ASK 1891
    // ASK 1890
    // ASK 1889 crossable
    // ASK 1888 crossable
    // ASK 1887 crossable
    // ASK 1886 crossable
    // ASK 1885 crossable
    // ASK 1884 crossable

    // BID 1889 crossable
    // BID 1888 crossable
    // BID 1887 crossable
    // BID 1886 crossable
    // BID 1885 crossable
    // BID 1884 crossable
    // BID 1883
    // BID 1882
    // BID 1881
    // BID 1880

    long price = (rand() % 10) + delta;
    
    long qty = ((rand() % 10) + 1) * 100;
      orders.push_back({i,side, price, qty});
  }
  
  clock_t start = clock();
  clock_t stop = start + (dur_sec * CLOCKS_PER_SEC);

  int count = run_test(order_book, orders, stop);
  if (count > 0) {
    std::cout << " - complete!" << std::endl;
    std::cout << "Inserted " << count << " orders in " << dur_sec << " seconds"
              << ", or " << count / dur_sec << " insertions per sec"
              << std::endl;
    std::cout << "Run matched " << order_book.GetMatch() << " orders\n" << std::endl;
      
    std::cout << "Note: To achieve best match result, please turn off(comment) the the stdout in MarketEvents" <<std::endl;
    return true;
  } else {
    std::cout << " - not enough orders" << std::endl;
    return false;
  }

  return count > 0;
}

int main(int argc, const char* argv[])
{
  uint32_t dur_sec = 3;
  if (argc > 1) {
    dur_sec = atoi(argv[1]);
    if (!dur_sec) {
      dur_sec = 3;
    }
  }
  std::cout << dur_sec << " sec performance test of order book" << std::endl;
  
  srand(dur_sec);

  {
    std::cout << "testing order book with depth" << std::endl;
    uint32_t num_to_try = dur_sec * 125000;
    while (true) {
      if (build_and_run_test(dur_sec, num_to_try)) {
        break;
      } else {
        num_to_try *= 2;
      }
    }
  }

}
