#pragma once

#include <iostream>
#include <vector>
#include "OrderListener.h"

namespace Matching {

class Event {
public:
	Event() {}
    virtual void ProcessEvent() = 0;
};

class AddEvent : public Event {
public:
    AddEvent(long orderId, Side side, long price, long size) :
    order_id_(orderId), side_(side), price_(price),
    size_(size), msgtype_(0) {}
    
    virtual void ProcessEvent() {
        std::cout<< "Message type:" << msgtype_ << " ,Add " << (side_==Buy?"Buy":"Sell") <<
        " order#:" << order_id_ << " @$" << price_ << " vol: " << size_ <<".\n";
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
	order_id_(order_id), msgtype_(1) {}
    
    virtual void ProcessEvent() {
        std::cout << "Message type:" << msgtype_ << " ,Cancel order#:" << order_id_ << " succeed.\n";
    }
    
    long order_id_;
    int msgtype_;
};

class TradeEvent : public Event {
public:
    TradeEvent(long price, long size) :
        price_(price), size_(size), msgtype_(2){}
    long price_;
    long size_;
    int msgtype_;
    virtual void ProcessEvent() {
        std::cout << "Message type:" << msgtype_ << " ,Trade @$" << price_ << " vol:" << size_<<"\n";
    }
};

class MatchEvent : public Event {
public:
    MatchEvent(long restingOrderId, long incomingOrderId, Side incomingSide,
               long price, long executedQuantity, long remainingQuantity) :
    resting_order_id_(restingOrderId), incoming_order_id_(incomingOrderId),
    side_(incomingSide), price_(price), executed_quantity_(executedQuantity),
    remaining_quantity_(remainingQuantity), msgtype_(3) {}
    
    virtual void ProcessEvent() {
        if(remaining_quantity_ == 0) {
            std::cout<< "Message type:" << 4 << ", PartialFilled order#:" <<
            incoming_order_id_ <<" left vol:" << executed_quantity_ << "\n";
            std::cout<< "Message type:" << 3 << ", FullyFilled order#:" <<
            resting_order_id_ <<"\n";
        }
        else {
            std::cout<< "Message type:" << msgtype_ << ", FullyFilled order#:" <<
            resting_order_id_ <<"\n";
            std::cout<< "Message type:" << 4 << ", PartialFilled order#:" <<
            incoming_order_id_ <<" left vol:" << remaining_quantity_ << "\n";
        }
    }
    
    long resting_order_id_;
    long incoming_order_id_;
    Side side_;
    long price_;
    long executed_quantity_;
    long remaining_quantity_;
    int msgtype_;
};


class MarketEvents : public OrderListener {
public:
	MarketEvents() {}

	const std::vector<Event*> GetEvents() const {
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
