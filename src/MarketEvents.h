//
//  MarketEvents.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include "OrderListener.h"

namespace Matching {

enum State {
    Add,
    Cancel,
    PartialFill,
    Fill,
    Complete
};

class Event {
public:
	Event(State state):state_(state) {}
    virtual void ProcessEvent() = 0;
    virtual bool EqualTo(Event* rhs) =0;
    State state_;
};


class AddEvent : public Event {
public:
    AddEvent(long orderId, Side side, long price, long size) :
        Event(Add), order_id_(orderId), side_(side),
        price_(price), size_(size), msgtype_(0) {}
    
    virtual void ProcessEvent() {
        std::cout<< "Message type:" << msgtype_ << " ,Add " << (side_==Buy?"Buy":"Sell") <<
        " order#:" << order_id_ << " @$" << price_ << " vol: " << size_ <<".\n";
    }
    
    virtual bool EqualTo(Event* rhs) {
        AddEvent* r = dynamic_cast<AddEvent*>(rhs);
        return this->order_id_==r->order_id_ &&
               this->side_==r->side_ &&
               this->price_==r->price_ &&
               this->size_==r->size_ &&
               this->state_==r->state_;
    }
    
    long order_id_;
    Side side_;
    long price_;
    long size_;
    int msgtype_;
};

class CancelEvent : public Event {
public:
	CancelEvent(long order_id) :
        Event(Cancel), order_id_(order_id), msgtype_(1) {}
    
    virtual void ProcessEvent() {
        std::cout << "Message type:" << msgtype_ << " ,Cancel order#:" << order_id_ << " succeed.\n";
    }
    
    virtual bool EqualTo(Event* rhs) {
        CancelEvent* r = dynamic_cast<CancelEvent*>(rhs);
        return this->order_id_==r->order_id_ &&
               this->state_==r->state_;
    }
    
    long order_id_;
    int msgtype_;
};

class TradeEvent : public Event {
public:
    TradeEvent(long price, long size) :
        Event(Complete), price_(price), size_(size), msgtype_(2){}
    long price_;
    long size_;
    int msgtype_;
    virtual void ProcessEvent() {
        std::cout << "Message type:" << msgtype_ << " ,Trade @$" << price_ << " vol:" << size_<<"\n";
    }
    
    virtual bool EqualTo(Event* rhs) {
        TradeEvent* r = dynamic_cast<TradeEvent*>(rhs);
        return this->price_==r->price_ &&
               this->size_==r->size_ &&
               this->state_==r->state_;
    }
};

class PartialFillEvent : public Event {
public:
    PartialFillEvent(long orderId, long remainingQuantity) :
        Event(PartialFill), order_id_(orderId),
        remaining_quantity_(remainingQuantity),msgtype_(4) {}
    
    virtual void ProcessEvent() {
        std::cout<< "Message type:" << msgtype_ << ", PartialFilled order#:" <<
        order_id_ <<" left vol:" << remaining_quantity_ << "\n";
    }
    
    virtual bool EqualTo(Event* rhs) {
        PartialFillEvent* r = dynamic_cast<PartialFillEvent*>(rhs);
        return this->order_id_==r->order_id_ &&
               this->remaining_quantity_==r->remaining_quantity_ &&
               this->state_==r->state_;
    }
    
    long order_id_;
    long remaining_quantity_;
    int msgtype_;
};

class FillEvent : public Event {
public:
    FillEvent(long orderId) :
    Event(Fill), order_id_(orderId), msgtype_(3) {}
    
    virtual void ProcessEvent() {
        std::cout<< "Message type:" << msgtype_ << ", FullyFilled order#:" << order_id_ <<"\n";
    }
    
    virtual bool EqualTo(Event *rhs) {
        FillEvent* r = dynamic_cast<FillEvent*>(rhs);
        return this->order_id_==r->order_id_ &&
               this->state_==r->state_;
    }
    
    long order_id_;
    int msgtype_;
};


class MarketEvents : public OrderListener {
public:
	MarketEvents() {}

	const std::vector<Event*>& GetEvents() const {
		return events_;
	}
    
    virtual void Add(long orderId, Side side, long price, long size);
    
    virtual void Match(long restingOrderId, long incomingOrderId, Side incomingSide,
                       long price, long executedQuantity, long remainingQuantity);
    
    virtual void Cancel(long orderId);

	virtual void Trade(long price, long size);

private:
	std::vector<Event*> events_;
};

}
