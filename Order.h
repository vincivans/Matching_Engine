//
//  Order.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/19/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Order_h
#define Order_h

#include "OrderBook.h"

namespace Matching {

class Order {
public:
    Order(OrderBook book, Side side, long price, long size):
         order_book_(book), side_(side), price_(price),
         remaining_quantity_(size) { }

    /**
     * Get the order book.
     *
     * @return the order book
     */
    OrderBook* GetOrderBook() {
        return order_book_;
    }

    /**
     * Get the price.
     *
     * @return the price
     */
    long GetPrice() {
        return price_;
    }

    /**
     * Get the side.
     *
     * @return the side
     */
    Side GetSide() {
        return side_;
    }

    /**
     * Get the remaining quantity.
     *
     * @return the remaining quantity
     */
    long GetRemainingQuantity() {
        return remaining_quantity_;
    }

    void SetRemainingQuantity(long remainingQuantity) {
        this.remaining_quantity_ = remainingQuantity;
    }

    void Reduce(long quantity) {
        remaining_quantity_ -= quantity;
    }
private:
    OrderBook* order_book_;

    Side side_;
    long price_;
    long remaining_quantity_;

}

}



#endif /* Order_h */
