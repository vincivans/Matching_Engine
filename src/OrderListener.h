//
//  OrderListener.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#pragma once
#include "Order.h"

namespace Matching {

class OrderListener {
public:
    // An event indicating that a trade has taken place.
    // @param book the order book
    // @param side the side of the incoming order
    // @param price the trade price
    // @param size the trade size
    virtual void Trade(long price, long size) = 0;
    
    
     // An event indicating that a cancel order request has taken place.
     // @param orderId the order identifier
    virtual void Cancel(long orderId) = 0;
    
    
     // Match an incoming order to a resting order in the order book. The match
     // occurs at the price of the order in the order book.
     // @param restingOrderId the order identifier of the resting order
     // @param incomingOrderId the order identifier of the incoming order
     // @param incomingSide the side of the incoming order
     // @param price the execution price
     // @param executedQuantity the executed quantity
     // @param remainingQuantity the remaining quantity of the resting order
    virtual void Match(long restingOrderId, long incomingOrderId, Side incomingSide,
                       long price, long executedQuantity, long remainingQuantity) = 0;
    
    
     // Add an order to the order book.
     // @param orderId the order identifier
     // @param side the side
     // @param price the limit price
     // @param size the size
    virtual void Add(long orderId, Side side, long price, long size) = 0;

};



}
