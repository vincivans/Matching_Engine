//
//  PriceLevel.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef PriceLevel_h
#define PriceLevel_h

#include <list>
#include <unordered_map>
#include "Order.h"

namespace Matching {

class PriceLevel {
public:
    PriceLevel(Side side, long price) :
        side_(side), price_(price) {}
    
    // Get the side.
    Side GetSide() const { return side_; }
    
    // Get the price.
    long GetPrice() const { return price_; }
    
    // Check if contains order at the pricelevel
    bool IsEmpty() const { return orders_.empty(); }
    
    // Get the order tree
    std::list<Order*>& GetOrderTree() { return orders_; }
    
    Order* Add(long order_id, long quantity);
    
    void Delete(long order_id) {
        auto todelete = prev(m_iter[order_id]);
        orders_.erase(todelete);
        m_iter.erase(order_id);
    }
    
private:
    Side side_;
    long price_;
    std::list<Order*> orders_;
    std::unordered_map<long, std::list<Order*>::iterator> m_iter;
};

inline
Order* PriceLevel::Add(long order_id, long quantity) {
       Order* order = new Order(side_, order_id, price_, quantity);
       orders_.emplace_back(order);
       m_iter[order_id] = orders_.end();
       return order;
}

}


#endif /* PriceLevel_h */
