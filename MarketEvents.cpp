#include "MarketEvents.h"

namespace Matching {
	
void MarketEvents::Trade(long price, long size) {
    events_.push_back(new TradeEvent(price, size));
    events_.back()->ProcessEvent();
}

void MarketEvents::Cancel(long orderId) {
    events_.push_back(new CancelEvent(orderId));
    events_.back()->ProcessEvent();
}

void MarketEvents::Add(long orderId, Side side, long price, long size) {
    events_.push_back(new AddEvent(orderId, side, price, size));
    events_.back()->ProcessEvent();
}

void MarketEvents::Match(long restingOrderId, long incomingOrderId, Side incomingSide, long price, long executedQuantity, long remainingQuantity) {
    events_.push_back(new MatchEvent(restingOrderId, incomingOrderId, incomingSide, price,
                                     executedQuantity, remainingQuantity));
    events_.back()->ProcessEvent();
}

}
