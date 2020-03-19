#include "stdafx.h"
#include <iostream>
#include "MarketEvents.h"

namespace Matching {
	
	void MarketEvents::Update(OrderBook* book, bool bbo) {
		events_.push_back(new UpdateEvent(book->GetInstrument(), bbo));
	}

	void MarketEvents::Trade(OrderBook* book, Side side, long price, long size) {
		events_.push_back(new TradeEvent(book->GetInstrument(), side, price, size));
		std::cout << "A Trade Event happened on " << side << " side, @ " <<
			price << " with qty " << size << "\n";
	}
}