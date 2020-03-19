//
//  OrderBook.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/19/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef OrderBook_h
#define OrderBook_h

#include <map>

namespace Matching {

enum Side {Buy, Sell};

class OrderBook {
public:
    OrderBook(long instrument) : instrument_(instrument) {}

    /**
     * Get the instrument.
     *
     * @return the instrument
     */
    long GetInstrument() {
        return instrument_;
    }

    /**
     * Get the best bid price.
     *
     * @return the best bid price or zero if there are no bids
     */
    long GetBestBidPrice() {
        if (bids_.empty())
            return 0;

        return *bids_.begin();
    }

    /**
     * Get the bid prices.
     *
     * @return the bid prices
     */
    public LongSortedSet getBidPrices() {
        return bids.keySet();
    }

    /**
     * Get a bid level size.
     *
     * @param price the bid price
     * @return the bid level size
     */
    long GetBidSize(long price) {
        return bids_[price];
    }

    /**
     * Get the best ask price.
     *
     * @return the best ask price or zero if there are no asks
     */
    long GetBestAskPrice() {
        if (asks_.empty())
            return 0;

        return *asks_.begin();
    }

    /**
     * Get the ask prices.
     *
     * @return the ask prices
     */
    public LongSortedSet getAskPrices() {
        return asks.keySet();
    }

    /**
     * Get an ask level size.
     *
     * @param price the ask price
     * @return the ask level size
     */
    long GetAskSize(long price) {
        return asks[price];
    }

    bool Add(Side side, long price, long quantity) {
        std::map<long, long>* levels = GetLevels(side);

        levels[price] = quantity;

        return price == *levels.begin();
    }

    boolean Update(Side side, long price, long quantity) {
        std::map<long, long>* levels = GetLevels(side);

        long oldSize = levels[price];
        long newSize = oldSize + quantity;

        bool onBestLevel = price == *levels.begin();

        if (newSize > 0)
            levels[price] = newSize;
        else
            levels.erase(price);

        return onBestLevel;
    }
private:
    std::map<long, long>* GetLevels(Side side) {
        return side == Side.BUY ? bids_ : asks_;
    }
    
    long instrument_;

    std::map<long, long>* bids_;
    std::map<long, long>* asks_;

}

}


#endif /* OrderBook_h */
