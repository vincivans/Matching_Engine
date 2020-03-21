#pragma once

#include <map>
#include <unordered_map>
#include "PriceLevel.h"
#include "MarketEvents.h"

namespace Matching {

typedef PriceLevel* PriceLevelPtr;
typedef std::map<long, PriceLevelPtr> PriceMap;
typedef std::unordered_map<long, Order*> OrderMap;

class OrderBook {
public:
    OrderBook(OrderListener* listener) : listener_(listener) {}

    
    // Get the best bid price.
    // @return the best bid price or zero if there are no bids
    PriceLevelPtr GetBestBidLevel() {
        if(bids_.empty()) return nullptr;
        return (bids_.rbegin())->second;
    }

    
    // Get the best ask price.
    // @return the best ask price or zero if there are no asks
    PriceLevelPtr GetBestAskLevel() {
        if (asks_.empty()) return nullptr;
        return (asks_.begin())->second;
    }
    
    
    // Enter an order to this order book
    // The incoming order is first matched against resting orders in this
    // order book. This operation results in zero or more Match events.
    // If the remaining quantity is not zero after the matching operation,
    // the remaining quantity is added to this order book and an Add event is
    // triggered.
    bool Enter(long order_id, Side side, long price, long qty);
    
    // Match orders base on the price level orders
    // Orders would be matched base on time when price level are the same.
    long Match(std::list<Order*>& orderlists, long order_id, Side side, long& quantity);
    
     // Cancel an order from order book.
     // An cancel event is triggered.
     // If the order identifier is unknown, do nothing.
     // @param orderId the order identifier
    void Cancel(long orderId);

private:
    void BuyOrder(long order_id, long price, long qty);
    void SellOrder(long order_id, long price, long qty);
    Order* AddAsk(long orderId, Side side, long price, long size);
    Order* AddBid(long orderId, Side side, long price, long size);
    
    
    PriceMap bids_;
    PriceMap asks_;
    OrderMap orders_;
    OrderListener* listener_;
};

inline
bool OrderBook::Enter(long order_id, Side side, long price, long qty) {
       if(orders_.count(order_id)) return false;
       if(side == Buy)
           BuyOrder(order_id, price, qty);
       else
           SellOrder(order_id, price, qty);
       return true;
}

inline
void OrderBook::Cancel(long orderId) {
    if(!orders_.count(orderId)) return;
    orders_.erase(orderId);
    listener_->Cancel(orderId);
}

inline
void OrderBook::BuyOrder(long order_id, long price, long qty) {
    long remainQuantity = qty;
    PriceLevelPtr bestLevel = GetBestAskLevel();
    while(remainQuantity > 0 && bestLevel && bestLevel->GetPrice()<=price) {
        remainQuantity = Match(bestLevel->GetOrderTree(), order_id, Buy, remainQuantity);
        if(!bestLevel) asks_.erase(bestLevel->GetPrice());
        bestLevel = GetBestAskLevel();
    }
    if(remainQuantity > 0) {
        orders_.emplace(order_id, AddBid(order_id, Buy, price, qty));
        listener_->Add(order_id, Buy, price, remainQuantity);
    }
}

inline
void OrderBook::SellOrder(long order_id, long price, long qty) {
    long remainQuantity = qty;
    PriceLevelPtr bestLevel = GetBestBidLevel();
    while(remainQuantity > 0 && bestLevel && bestLevel->GetPrice()>=price) {
        remainQuantity = Match(bestLevel->GetOrderTree(), order_id, Sell, remainQuantity);
        if(!bestLevel) bids_.erase(bestLevel->GetPrice());
        bestLevel = GetBestBidLevel();
    }
    if(remainQuantity > 0) {
        orders_.emplace(order_id, AddAsk(order_id, Sell, price, qty));
        listener_->Add(order_id, Sell, price, remainQuantity);
    }
}

inline
long OrderBook::Match(std::list<Order*>& orderlists, long order_id,
                      Side side, long& quantity) {
    while(quantity>0 && !orderlists.empty()) {
        Order* resting = *(orderlists).begin();
        long restingId = resting->GetId();
        long restingQuantity = resting->GetRemainingQuantity();
        long price = resting->GetPrice();
        if(restingQuantity > quantity) {
            resting->Reduce(quantity);
            //partial fill
            listener_->Match(restingId, order_id, side, price,
                            quantity,resting->GetRemainingQuantity());
            quantity = 0;
        }
        else {
            orderlists.pop_front();
            orders_.erase(restingId);
            //full fill
            quantity -= restingQuantity;
            listener_->Trade(price, restingQuantity);
            listener_->Match(restingId, order_id, side, price, quantity, 0);
        }
    }
    return quantity;
}


inline
Order* OrderBook::AddAsk(long orderId, Side side, long price, long size) {
    PriceLevelPtr level = nullptr;
    if(!asks_.count(price)) {
        level = new PriceLevel(side, price);
        asks_[price] = level;
    }
    else {
        level = asks_[price];
    }
    Order* order = level->Add(orderId, size);
    return order;
}

inline
Order* OrderBook::AddBid(long orderId, Side side, long price, long size) {
    PriceLevelPtr level = nullptr;
    if(!bids_.count(price)) {
        level = new PriceLevel(side, price);
        bids_[price] = level;
    }
    else {
        level = bids_[price];
    }
    Order* order = level->Add(orderId, size);
    return order;
}


}
