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
static long INSTRUMENT = 1;


int main(int argc, const char * argv[]) {
    MarketEvents* me = new MarketEvents();
    /*Market market(me);
    OrderBook* book = market.Open(INSTRUMENT);
    
    market.Add(INSTRUMENT, 1, Buy, 999, 100);
    market.Add(INSTRUMENT, 2, Sell, 1001, 200);
    market.Add(INSTRUMENT, 3, Sell, 1002,  50);
    market.Execute(2, 200);*/
    OrderBook *book = new OrderBook(me);
    book->Enter(1, Buy, 1000, 100);
    Event* bid = new AddEvent(1, Buy, 1000, 100);
    return 0;
}
