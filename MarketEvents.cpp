//
//  MarketEvents.cpp
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//


#include "MarketEvents.h"

namespace Matching {
	
void MarketEvents::Trade(long price, long size) {
    events_.push_back(new TradeEvent(price, size));
    events_.back()->ProcessEvent();
}

void MarketEvents::Cancel(long orderId) {
    events_.push_back(new CancelEvent(orderId));
    events_.back()->ProcessEvent();
}

void MarketEvents::Add(long orderId, Side side, long price, long size) {
    events_.push_back(new AddEvent(orderId, side, price, size));
    events_.back()->ProcessEvent();
}

void MarketEvents::Match(long restingOrderId, long incomingOrderId, Side incomingSide, long price, long executedQuantity, long remainingQuantity) {
    if(remainingQuantity == 0) {
        if(executedQuantity != remainingQuantity) {
            events_.push_back(new PartialFillEvent(incomingOrderId, executedQuantity));
            events_.back()->ProcessEvent();
            events_.push_back(new FillEvent(restingOrderId));
            events_.back()->ProcessEvent();
        }
        else {
            events_.push_back(new FillEvent(incomingOrderId));
            events_.back()->ProcessEvent();
            events_.push_back(new FillEvent(restingOrderId));
            events_.back()->ProcessEvent();
        }
    }
    else {
        events_.push_back(new FillEvent(incomingOrderId));
        events_.back()->ProcessEvent();
        events_.push_back(new PartialFillEvent(restingOrderId, remainingQuantity));
        events_.back()->ProcessEvent();
    }
}

}
