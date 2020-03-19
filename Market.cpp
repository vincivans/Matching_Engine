//
//  Market.cpp
//  Matching_Simulation
//
//  Created by SiranWang on 3/19/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#include "Market.h"

namespace Matching {

/**
 * Open an order book.
 *
 * If the order book for the instrument is already open, do nothing.
 *
 * @param instrument an instrument
 *
 */
void Market::Open(long instrument) {
    if(!books_->count(instrument))
        books_[instrument] = new OrderBook(instrument);
}

/**
 * Find an order.
 *
 * @param orderId the order identifier
 * @return the order or {@code null} if the order identifier is unknown
 */
Order* Market::Find(long orderId) {
    return orders_[orderId];
}

/**
 * Add an order to an order book.
 *
 * <p>An update event is triggered.</p>
 *
 * <p>If the order book for the instrument is closed or the order
 * identifier is known, do nothing.</p>
 *
 * @param instrument the instrument
 * @param orderId the order identifier
 * @param side the side
 * @param price the price
 * @param size the size
 */
void Add(long instrument, long orderId, Side side, long price, long size) {
    if (orders_->count(orderId))
        return;

    if(!book_->count(instrument)) return;

    Order* order = new Order(book, side, price, size);

    bool bbo = book.add(side, price, size);

    orders_[orderId] = order;

    listener.update(book, bbo);
}

}
