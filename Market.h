//
//  Market.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/19/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Market_h
#define Market_h

#include <unordered_map>
#include "OrderBook.h"
#include "Order.h"

namespace Matching {

class Market {
    /**
     * Create a market.
     *
     * @param listener a listener for outbound events from the market
     */
    Market(MarketListener listener) {
        this.books  = new Long2ObjectArrayMap<>();
        this.orders = new Long2ObjectOpenHashMap<>();

        this.listener = listener;
    }

    void Open(long instrument);

    Order* Find(long orderId);

    void Add(long instrument, long orderId, Side side, long price, long size);
    
    /**
     * Modify an order in an order book. The order will retain its time
     * priority. If the new size is zero, the order is deleted from the
     * order book.
     *
     * <p>An update event is triggered.</p>
     *
     * <p>If the order identifier is unknown, do nothing.</p>
     *
     * @param orderId the order identifier
     * @param size the new size
     */
    void Modify(long orderId, long size) {
        Order order = orders.get(orderId);
        if (order == null)
            return;

        OrderBook book = order.getOrderBook();

        long newSize = Math.max(0, size);

        boolean bbo = book.update(order.getSide(), order.getPrice(),
                newSize - order.getRemainingQuantity());

        if (newSize == 0)
            orders.remove(orderId);
        else
            order.setRemainingQuantity(newSize);

        listener.update(book, bbo);
    }

    /**
     * Execute a quantity of an order in an order book. If the remaining
     * quantity reaches zero, the order is deleted from the order book.
     *
     * <p>A Trade event and an update event are triggered.</p>
     *
     * <p>If the order identifier is unknown, do nothing.</p>
     *
     * @param orderId the order identifier
     * @param quantity the executed quantity
     * @return the remaining quantity
     */
    long Execute(long orderId, long quantity) {
        Order order = orders.get(orderId);
        if (order == null)
            return 0;

        return execute(orderId, order, quantity, order.getPrice());
    }

    /**
     * Execute a quantity of an order in an order book. If the remaining
     * quantity reaches zero, the order is deleted from the order book.
     *
     * <p>A Trade event and an update event are triggered.</p>
     *
     * <p>If the order identifier is unknown, do nothing.</p>
     *
     * @param orderId the order identifier
     * @param quantity the executed quantity
     * @param price the execution price
     * @return the remaining quantity
     */
    long Execute(long orderId, long quantity, long price) {
        Order order = orders.get(orderId);
        if (order == null)
            return 0;

        return execute(orderId, order, quantity, price);
    }

    long Execute(long orderId, Order order, long quantity, long price) {
        OrderBook book = order.getOrderBook();

        Side side = order.getSide();

        long remainingQuantity = order.getRemainingQuantity();

        long executedQuantity = Math.min(quantity, remainingQuantity);

        listener.trade(book, contra(side), price, executedQuantity);

        book.update(side, order.getPrice(), -executedQuantity);

        if (executedQuantity == remainingQuantity)
            orders.remove(orderId);
        else
            order.reduce(executedQuantity);

        listener.update(book, true);

        return remainingQuantity - executedQuantity;
    }

    /**
     * Cancel a quantity of an order in an order book. If the remaining
     * quantity reaches zero, the order is deleted from the order book.
     *
     * <p>An update event is triggered.</p>
     *
     * <p>If the order identifier is unknown, do nothing.</p>
     *
     * @param orderId the order identifier
     * @param quantity the canceled quantity
     * @return the remaining quantity
     */
    long Cancel(long orderId, long quantity) {
        Order order = orders.get(orderId);
        if (order == null)
            return 0;

        OrderBook book = order.getOrderBook();

        long remainingQuantity = order.getRemainingQuantity();

        long canceledQuantity = Math.min(quantity, remainingQuantity);

        boolean bbo = book.update(order.getSide(), order.getPrice(), -canceledQuantity);

        if (canceledQuantity == remainingQuantity)
            orders.remove(orderId);
        else
            order.reduce(canceledQuantity);

        listener.update(book, bbo);

        return remainingQuantity - canceledQuantity;
    }

    /**
     * Delete an order from an order book.
     *
     * <p>An update event is triggered.</p>
     *
     * <p>If the order identifier is unknown, do nothing.</p>
     *
     * @param orderId the order identifier
     */
    void Delete(long orderId) {
        Order order = orders.get(orderId);
        if (order == null)
            return;

        OrderBook book = order.getOrderBook();

        boolean bbo = book.update(order.getSide(), order.getPrice(), -order.getRemainingQuantity());

        orders.remove(orderId);

        listener.update(book, bbo);
    }
private:
    
    private static Side contra(Side side) {
        return side == Side.BUY ? Side.SELL : Side.BUY;
    }
    std::unordered_map<long, OrderBook*>* books_;

    std::unordered_map<long, Order*>* orders_;

    MarketEvents listener;

}

}

#endif /* Market_h */
