//
//  main.cpp
//  Matching_Simulation
//
//  Created by SiranWang on 3/19/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

//#include <iostream>
#include "OrderBook.h"
using namespace Matching;
static long id = 1;


int main(int argc, const char * argv[]) {
    MarketEvents* me = new MarketEvents();
    /*Market market(me);
    OrderBook* book = market.Open(INSTRUMENT);
    
    market.Add(INSTRUMENT, 1, Buy, 999, 100);
    market.Add(INSTRUMENT, 2, Sell, 1001, 200);
    market.Add(INSTRUMENT, 3, Sell, 1002,  50);
    market.Execute(2, 200);*/
    OrderBook *book = new OrderBook(me);
    book->Enter(1, Sell, 1075, 1);
    book->Enter(2, Buy, 1000, 9);
    book->Enter(3, Buy, 975, 30);
    book->Enter(4, Sell, 1050, 10);
    book->Enter(5, Buy, 950, 10);
    book->Enter(6, Sell, 1025, 2);
    book->Enter(7, Buy, 1000, 1);
    book->Enter(8, Sell, 1025, 5);
    book->Enter(9, Buy, 1050, 3);
    return 0;
}
