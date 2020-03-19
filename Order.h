#pragma once

#include "OrderBook.h"

namespace Matching {

	class Order {
	public:
		Order(OrderBook* book, Side side, long price, long size) :
			order_book_(book), side_(side), price_(price),
			remaining_quantity_(size) { }

		/**
		* Get the order book.
		*
		* @return the order book
		*/
		OrderBook* GetOrderBook() const {
			return order_book_;
		}

		/**
		* Get the price.
		*
		* @return the price
		*/
		long GetPrice() const {
			return price_;
		}

		/**
		* Get the side.
		*
		* @return the side
		*/
		Side GetSide() const {
			return side_;
		}

		/**
		* Get the remaining quantity.
		*
		* @return the remaining quantity
		*/
		long GetRemainingQuantity() const {
			return remaining_quantity_;
		}

		void SetRemainingQuantity(long remainingQuantity) {
			remaining_quantity_ = remainingQuantity;
		}

		void Reduce(long quantity) {
			remaining_quantity_ -= quantity;
		}
	private:
		OrderBook* order_book_;
		Side side_;
		long price_;
		long remaining_quantity_;

	};

}

