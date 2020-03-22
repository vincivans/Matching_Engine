//
//  main.cpp
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//


#include "MatchSimulation.h"
using namespace Matching;
using namespace std;


int main(int argc, const char * argv[]) {
    /*MarketEvents* me = new MarketEvents();
    OrderBook *book = new OrderBook(me);
    book->Enter(1, Sell, 1075, 1);
    book->Enter(2, Buy, 1000, 9);
    book->Enter(3, Buy, 975, 30);
    book->Enter(4, Sell, 1050, 10);
    book->Enter(5, Buy, 950, 10);
    book->Enter(6, Sell, 1025, 2);
    book->Enter(7, Buy, 1000, 1);
    book->Enter(8, Sell, 1025, 5);
    book->Enter(9, Buy, 1050, 3);*/
    MatchSimulation ms;
    ms.Run();
    return 0;
}



