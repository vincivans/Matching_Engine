#pragma once

#include <unordered_map>
#include "MarketEvents.h"
#include "OrderBook.h"
#include "Order.h"

namespace Matching {

	class Market {
	public:
		/**
		* Create a market.
		*
		* @param listener a listener for outbound events from the market
		*/
		Market(MarketEvents* listener) : listener_(listener) {}

		OrderBook* Open(long instrument);

		Order* Find(long orderId) ;

		void Add(long instrument, long orderId, Side side, long price, long size);
		void Modify(long orderId, long size);
		long Execute(long orderId, long quantity);
		long Execute(long orderId, long quantity, long price);
		long Execute(long orderId, Order* order, long quantity, long price);
		long Cancel(long orderId, long quantity);
		void Delete(long orderId);

	private:
		Side Opposite(Side side) const {
			return side == Buy? Sell : Buy;
		}
		std::unordered_map<long, OrderBook*> books_;

		std::unordered_map<long, Order*> orders_;

		MarketListener* listener_;
	};

}