//
//  TestMatching.cpp
//  Test
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "TestUtils.h"
using namespace std;
using namespace Matching;


TEST_CASE("Test Matching Simulation") {
    MarketEvents* me = new MarketEvents();
    OrderBook* book = new OrderBook(me);
    
    SECTION("Test No Match") {
        book->Enter(1, Buy, 1000, 100);
        book->Enter(2, Buy, 1100, 100);
        book->Enter(3, Sell, 1200, 100);
        
        Event* bid1 = new AddEvent(1, Buy, 1000, 100);
        Event* bid2 = new AddEvent(2, Buy, 1100, 100);
        Event* ask = new AddEvent(3, Sell, 1200, 100);
        CHECK(EventsEqual(me->GetEvents(), {bid1, bid2, ask}));
    }
    
    SECTION("Test Buy") {
        book->Enter(1, Sell, 1000, 100);
        book->Enter(2, Buy, 1000, 100);
        
        Event* ask = new AddEvent(1, Sell, 1000, 100);
        Event* trade = new TradeEvent(1000, 100);
        Event* fill = new FillEvent(2);
        Event* fill2 = new FillEvent(1);
        CHECK(EventsEqual(me->GetEvents(), {ask, trade, fill, fill2}));
    }
    
    SECTION("Test Sell") {
        book->Enter(1, Buy, 1000, 100);
        book->Enter(2, Sell, 1000, 100);
        
        Event* bid = new AddEvent(1, Buy, 1000, 100);
        Event* trade = new TradeEvent(1000, 100);
        Event* fill = new FillEvent(2);
        Event* fill2 = new FillEvent(1);
        CHECK(EventsEqual(me->GetEvents(), {bid, trade, fill, fill2}));
    }
    
    SECTION("Test Multilevel Buy") {
        book->Enter(1, Sell, 1000, 100);
        book->Enter(2, Sell, 1001, 100);
        book->Enter(3, Sell, 999, 50);
        book->Enter(4, Buy, 1000, 100);
        
        Event* ask1 = new AddEvent(1, Sell, 1000, 100);
        Event* ask2 = new AddEvent(2, Sell, 1001, 100);
        Event* ask3 = new AddEvent(3, Sell, 999, 50);
        Event* trade = new TradeEvent(999, 50);
        Event* pfill = new PartialFillEvent(4, 50);
        Event* fill = new FillEvent(3);
        Event* trade2 = new TradeEvent(1000, 50);
        Event* fill2 = new FillEvent(4);
        Event* pfill2 = new PartialFillEvent(1, 50);
        CHECK(EventsEqual(me->GetEvents(), {ask1,ask2,ask3,trade,pfill,fill,trade2,fill2,pfill2}));
    }
    
    SECTION("Test Multilevel Sell") {
        book->Enter(1, Buy, 1000, 100);
        book->Enter(2, Buy, 999, 100);
        book->Enter(3, Buy, 1001, 50);
        book->Enter(4, Sell, 1000, 100);
        
        Event* bid1 = new AddEvent(1, Buy, 1000, 100);
        Event* bid2 = new AddEvent(2, Buy, 999, 100);
        Event* bid3 = new AddEvent(3, Buy, 1001, 50);
        Event* trade = new TradeEvent(1001, 50);
        Event* pfill = new PartialFillEvent(4, 50);
        Event* fill = new FillEvent(3);
        Event* trade2 = new TradeEvent(1000, 50);
        Event* fill2 = new FillEvent(4);
        Event* pfill2 = new PartialFillEvent(1, 50);
        CHECK(EventsEqual(me->GetEvents(), {bid1,bid2,bid3,trade,pfill,fill,trade2,fill2,pfill2}));
    }
    
    SECTION("Test Partial Bid Fill") {
        book->Enter(1, Buy, 1000, 100);
        book->Enter(2, Sell, 1000, 50);
        book->Enter(3, Sell, 1000, 50);
        book->Enter(4, Sell, 1000, 50);
        
        Event* bid = new AddEvent(1, Buy, 1000, 100);
        Event* trade = new TradeEvent(1000, 50);
        Event* fill = new FillEvent(2);
        Event* pfill = new PartialFillEvent(1, 50);
        Event* trade2 = new TradeEvent(1000, 50);
        Event* fill2 = new FillEvent(3);
        Event* fill3 = new FillEvent(1);
        Event* ask = new AddEvent(4, Sell, 1000, 50);
        CHECK(EventsEqual(me->GetEvents(), {bid,trade,fill,pfill,trade2,fill2,fill3, ask}));
    }
    
    SECTION("Test Partial Ask Fill") {
        book->Enter(1, Sell, 1000, 100);
        book->Enter(2, Buy, 1000, 50);
        book->Enter(3, Buy, 1000, 50);
        book->Enter(4, Buy, 1000, 50);
        
        Event* ask = new AddEvent(1, Sell, 1000, 100);
        Event* trade = new TradeEvent(1000, 50);
        Event* fill = new FillEvent(2);
        Event* pfill = new PartialFillEvent(1, 50);
        Event* trade2 = new TradeEvent(1000, 50);
        Event* fill2 = new FillEvent(3);
        Event* fill3 = new FillEvent(1);
        Event* bid = new AddEvent(4, Buy, 1000, 50);
        CHECK(EventsEqual(me->GetEvents(), {ask,trade,fill,pfill,trade2,fill2,fill3, bid}));
    }
    
    
    SECTION("Test Cancel") {
        book->Enter(1, Buy, 1000, 100);
        book->Cancel(1);
        book->Enter(2, Sell, 1000, 100);
        
        Event* bid = new AddEvent(1,Buy,1000,100);
        Event* cancel = new CancelEvent(1);
        Event* ask = new AddEvent(2,Sell,1000,100);
        
        CHECK(EventsEqual({bid, cancel, ask}, me->GetEvents()));
    }
    

}
