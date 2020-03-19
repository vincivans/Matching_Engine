#pragma once

#include "OrderBook.h"

namespace Matching {

	class MarketListener {
	public:
		MarketListener() {}


		//An event indicating that an order book has changed.
		//@param book the order book
		//@param bbo true if the best bid and offer (BBO) has changed, otherwise false
		virtual void Update(OrderBook* orderBook, bool bbo) = 0;


		// An event indicating that a trade has taken place.
		// @param book the order book
		// @param side the side of the incoming order
		// @param price the trade price
		// @param size the trade size
		virtual void Trade(OrderBook* orderBook, Side side, long price, long size) = 0;

	};

}
