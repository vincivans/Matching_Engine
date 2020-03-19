//
//  MatchingEngine.hpp
//  Matching_Engine
//
//  Created by SiranWang on 3/14/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef MatchingEngine_h
#define MatchingEngine_h

#include "Simulation.h"
#include "Orderbook.h"

namespace Matching {

/*class MatchingEngine {
public:
    MatchingEngine();
    virtual ~MatchingEngine() { Clean(); }
    
    void Clean() { delete order_book_; }
    int Run(std::ostream & out = std::cout);
    
    const OrderBook* GetOrderBook() const { return order_book_; }
    
    void ProcessOrder(Order* order);
    void CancelOrder(long order_id);
private:
    OrderBook* order_book_;
};*/

typedef std::map<long, Order*> OrderMap;

class MatchingEngine : public Simulation {
public:
    MatchingEngine() : Simulation() {}
    
    void AddOrder(Order* order);
    void CancelOrder(Order* order);
    void ProcessOrder();
    
    const OrderBook* GetOrderBook() const { return order_book_; }
private:
    OrderBook* order_book_;
    OrderMap orders_;
} matching_engine_;

}

#endif /* MatchingEngine_hpp */
