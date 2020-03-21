#include "MarketEvents.h"

namespace Matching {
	
/*void MarketEvents::Update(OrderBook* book, bool bbo) {
    events_.push_back(new UpdateEvent(book->GetInstrument(), bbo));
}

void MarketEvents::Trade(OrderBook* book, Side side, long price, long size) {
    std::cout << "A Trade Event happened on " << (side==Buy?"Buy":"Sell") << " side, @ " <<
    price << " with qty " << size << "\n";
    events_.push_back(new TradeEvent(book->GetInstrument(), side, price, size));
}*/

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
