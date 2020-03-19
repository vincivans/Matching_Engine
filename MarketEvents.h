#pragma once

#include <vector>
#include "MarketListener.h"

namespace Matching {

class Event {
public:
	Event() {}
};



class UpdateEvent : public Event {
public:
	UpdateEvent(long instrument, bool bbo) :
	instrument_(instrument), bbo_(bbo) {}
	long instrument_;
	bool bbo_;
};

class TradeEvent : public Event {
public:
	TradeEvent(long instrument, Side side, long price, long size) :
		instrument_(instrument), side_(side),
		price_(price), size_(size) {}
	long instrument_;
	Side side_;
	long price_;
	long size_;
};

class MarketEvents : public MarketListener {
public:
	MarketEvents() {}

	const std::vector<Event*> GetEvents() const {
		return events_;
	}

	virtual void Update(OrderBook* book, bool bbo);

	virtual void Trade(OrderBook* book, Side side, long price, long size);

private:
	std::vector<Event*> events_;
};

}