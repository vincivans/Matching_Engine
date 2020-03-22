//
//  Order.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#pragma once

namespace Matching {

enum Side { Buy, Sell };

class Order {
public:
    Order(Side side, long order_id, long price, long size) :
        side_(side), price_(price), order_id_(order_id),
        remaining_quantity_(size) { }
    
    // Get the order id.
    // @return the order id
    long GetId() const { return order_id_; }

    
    // Get the price.
    // @return the price
    long GetPrice() const { return price_; }


    // Get the side.
    // @return the side
    Side GetSide() const { return side_; }

    // Get the remaining quantity.
    // @return the remaining quantity
    long GetRemainingQuantity() const { return remaining_quantity_; }
    
    // Set the remaining quantity.
    void SetRemainingQuantity(long remainingQuantity) {
        remaining_quantity_ = remainingQuantity;
    }

    void Reduce(long quantity) {
        remaining_quantity_ -= quantity;
    }
private:
    Side side_;
    long order_id_;
    long price_;
    long remaining_quantity_;
};

}

