#include "stdafx.h"
#include <algorithm>
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
	OrderBook* Market::Open(long instrument) {
		if (!books_.count(instrument))
			books_[instrument] = new OrderBook(instrument);
		return books_[instrument];
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
	void Market::Add(long instrument, long orderId, Side side, long price, long size) {
		if (orders_.count(orderId))
			return;

		if (!books_.count(instrument)) return;
		OrderBook* book = books_[instrument];

		Order* order = new Order(book, side, price, size);

		bool bbo = book->Add(side, price, size);

		orders_[orderId] = order;

		listener_->Update(book, bbo);
	}


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
	void Market::Modify(long orderId, long size) {
		if (!orders_.count(orderId)) return;

		Order* order = orders_[orderId];
		OrderBook* book = order->GetOrderBook();

		long newSize = std::max((long)0, size);

		bool bbo = book->Update(order->GetSide(), order->GetPrice(),
			newSize - order->GetRemainingQuantity());

		if (newSize == 0)
			orders_.erase(orderId);
		else
			order->SetRemainingQuantity(newSize);

		listener_->Update(book, bbo);
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
	long Market::Execute(long orderId, long quantity) {
		if (!orders_.count(orderId))
			return 0;
		Order* order = orders_[orderId];

		return Execute(orderId, order, quantity, order->GetPrice());
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
	long Market::Execute(long orderId, long quantity, long price) {
		if (!orders_.count(orderId))
			return 0;
		Order* order = orders_[orderId];

		return Execute(orderId, order, quantity, price);
	}

	long Market::Execute(long orderId, Order* order, long quantity, long price) {
		OrderBook* book = order->GetOrderBook();

		Side side = order->GetSide();

		long remainingQuantity = order->GetRemainingQuantity();

		long executedQuantity = std::min(quantity, remainingQuantity);

		listener_->Trade(book, Opposite(side), price, executedQuantity);

		book->Update(side, order->GetPrice(), -executedQuantity);

		if (executedQuantity == remainingQuantity)
			orders_.erase(orderId);
		else
			order->Reduce(executedQuantity);

		listener_->Update(book, true);

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
	long Market::Cancel(long orderId, long quantity) {
		if (!orders_.count(orderId))
			return 0;
		Order* order = orders_[orderId];

		OrderBook* book = order->GetOrderBook();

		long remainingQuantity = order->GetRemainingQuantity();

		long canceledQuantity = std::min(quantity, remainingQuantity);

		bool bbo = book->Update(order->GetSide(), order->GetPrice(), -canceledQuantity);

		if (canceledQuantity == remainingQuantity)
			orders_.erase(orderId);
		else
			order->Reduce(canceledQuantity);

		listener_->Update(book, bbo);

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
	void Market::Delete(long orderId) {
		if (!orders_.count(orderId)) return;
		Order* order = orders_[orderId];

		OrderBook* book = order->GetOrderBook();

		bool bbo = book->Update(order->GetSide(), order->GetPrice(), -order->GetRemainingQuantity());

		orders_.erase(orderId);

		listener_->Update(book, bbo);
	}
}