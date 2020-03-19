//
//  MarketEvents.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/19/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef MarketEvents_h
#define MarketEvents_h

#include "OrderBook.h"

namespace Matching {

class MarketEvents {
public:
    
    //An event indicating that an order book has changed.
    //@param book the order book
    //@param bbo true if the best bid and offer (BBO) has changed, otherwise false
    virtual Update(OrderBook* orderBook, bool bbo);
    
    
    // An event indicating that a trade has taken place.
    // @param book the order book
    // @param side the side of the incoming order
    // @param price the trade price
    // @param size the trade size
    virtual Trade(OrderBook* orderBook, Side side, long price, long size);

};

}

#endif /* MarketEvents_h */
