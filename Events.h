//
//  Events.h
//  Matching_Engine
//
//  Created by SiranWang on 3/17/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Events_h
#define Events_h

#include "Event.h"
#include "Orderbook.h"

namespace Matching {

class AddOrderRequest : public Event {
public:
    AddOrderRequest() {}
    virtual void ProcessEvent();
};

class CancelOrderRequest : public Event {
public:
    CancelOrderRequest() {}
    virtual void ProcessEvent();
};

class TradeEvent : public Event {
public:
    TradeEvent() {}
    virtual void ProcessEvent();
};

class OrderFullyFilled : public Event {
public:
    OrderFullyFilled() {}
    virtual void ProcessEvent();
};

class OrderPartiallyFilled : public Event {
public:
    OrderPartiallyFilled() {}
    virtual void ProcessEvent();
};

}

#endif /* Events_h */
