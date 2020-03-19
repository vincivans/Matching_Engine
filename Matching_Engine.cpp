// Matching_Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Market.h"
using namespace std;
using namespace Matching;

static long order_id = 1;

int main()
{
	MarketEvents* me = new MarketEvents();
	Market market(me);
	OrderBook* book = market.Open(order_id);

	market.Add(order_id, 1, Buy, 999, 100);
	market.Add(order_id, 2, Sell, 1001, 200);
}

