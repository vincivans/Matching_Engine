//
//  Orderbook.h
//  Matching_Engine
//
//  Created by SiranWang on 3/14/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Orderbook_h
#define Orderbook_h

#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <limits>
#include <iostream>
#include "OrderFwd.h"

namespace Matching {
#define MIN std::numeric_limits<int>::min()

//template <class OrderPtr>
class PriceNode;

//Place order with larger qty and earlier arrives
struct OrderComparator {
    bool operator()(const Order* lhs, const Order* rhs) {
        return lhs->GetQuantity() < rhs->GetQuantity();
    }
};

typedef PriceNode* PriceNodePtr;
typedef std::map<int, PriceNodePtr> PriceTree;
typedef std::map<int, PriceNodePtr>::iterator PriceTreeIt;
typedef std::map<int, PriceNodePtr>::reverse_iterator PriceTreeRevIt;
typedef std::unordered_map<int, PriceNodePtr> PriceToNodeMap;
typedef std::unordered_map<int, PriceNodePtr>::iterator PriceToNodeMapIt;
typedef std::set<Order*, OrderComparator> OrderTree;
typedef std::set<Order*, OrderComparator>::iterator OrderTreeIt;


class PriceNode {
public:
    PriceNode() : price_(MIN), order_tree_(nullptr) {}
    PriceNode(int price) : price_(price) { order_tree_ = new OrderTree(); }
    virtual ~PriceNode() { Clean(); }
    void Clean();
    
    int GetPrice() { return price_; }
    OrderTree*& GetOrderTree() { return order_tree_; }
    void InsertOrder(Order* order) { order_tree_->emplace(order); }
    
private:
    int price_;
    OrderTree* order_tree_;
};

//Template to allows user to supply common or smart pointers
//template <typename OrderPtr>
class OrderBook {
public:
    OrderBook();
    virtual ~OrderBook();
    
    void Add(Order* order);
    void Match(const Order* order, int& quantity_to_match);
    void Match(OrderTreeIt it, const Order* order, int& qtyToMatch,
               OrderTree* quotes, int bestPrice);
    void Cancel(Order* order);
    bool FindOnMarket(Order* order, PriceToNodeMapIt& it);
    
    PriceTree*& GetBidTree()        { return bid_tree_; }
    PriceTree*& GetAskTree()        { return ask_tree_; }
    PriceToNodeMap*& GetBidMap()    { return bid_map_;  }
    PriceToNodeMap*& GetAskMap()    { return ask_map_;  }
    
    bool IsMarketable(const Order* order, int best_price, bool is_buy);
private:
    //Sorted tree for indexing bid and ask orders within order book
    //PriceNode contains all quotes in size>time order using another BST for that price level
    PriceTree* bid_tree_; // < price, LimitPriceNode >
    PriceTree* ask_tree_;

    //Hashmap to speed up Add() in order book to O(1) if price already exists
    PriceToNodeMap* bid_map_; // < price, LimitPriceNode >
    PriceToNodeMap* ask_map_;
};



//PriceNode
inline
void PriceNode::Clean() {
    for(OrderTreeIt it = order_tree_->begin(); it != order_tree_->end(); ++it )
        delete *it;
    delete order_tree_;
}


//OrderBook
inline
OrderBook::OrderBook() {
    bid_tree_ = new PriceTree();
    ask_tree_ = new PriceTree();
    bid_map_ = new PriceToNodeMap();
    ask_map_ = new PriceToNodeMap();
}

inline
OrderBook::~OrderBook() {
    for(PriceTreeIt it = bid_tree_->begin(); it != bid_tree_->end(); ++it )
        delete it->second;
    for(PriceTreeIt it = ask_tree_->begin(); it != ask_tree_->end(); ++it )
        delete it->second;

    delete bid_tree_;
    delete ask_tree_;
    delete bid_map_;
    delete ask_map_;
}

inline
bool OrderBook::IsMarketable(const Order* order, int best_price, bool is_buy ) {
    if(is_buy)
        return order->GetPrice() >= best_price;
    else
        return order->GetPrice() <= best_price;
}


//Marketable order handling:
//Remove liquidity to the other side of the book given and order
//time: O(1)
inline
void OrderBook::Match(const Order* order, int& qty_to_match) {
    bool isBuy = order->Is_Buy();

    //Get opposite side of tree to match
    if(isBuy) {
        for(PriceTreeIt itBestPrice = ask_tree_->begin();
                itBestPrice != ask_tree_->end() &&
                IsMarketable( order, ask_tree_->begin()->first, order->Is_Buy() ) &&
                qty_to_match > 0; ) {
            int bestPrice = itBestPrice->first;
            PriceNode* bestPriceNode = itBestPrice->second;
            OrderTree* quotes = bestPriceNode->GetOrderTree();

            //For each order (in size > time sequence) in this price level
            Match(quotes->begin(), order, qty_to_match, quotes, bestPrice);

            //Order depletes current price level
            if( quotes->empty() ) {
                delete bestPriceNode;
                //bestPriceNode = NULL;
                ask_tree_->erase( itBestPrice++ );
                ask_map_->erase( bestPrice );
            }
            else
                ++itBestPrice;
        }
    }
    else {
        for(PriceTreeRevIt itBestPrice = bid_tree_->rbegin();
                itBestPrice != bid_tree_->rend() &&
                IsMarketable( order, bid_tree_->rbegin()->first, order->Is_Buy() ) &&
                qty_to_match > 0;  ) {
            int bestPrice = itBestPrice->first;
            PriceNode* bestPriceNode = itBestPrice->second;
            OrderTree* quotes = bestPriceNode->GetOrderTree();

            // for each order (in size > time sequence) in this price level
            Match(quotes->begin(), order, qty_to_match, quotes, bestPrice);

            // order depletes current price level
            if(quotes->empty() ) {
                delete bestPriceNode;
                //bestPriceNode = NULL;
                bid_tree_->erase( std::next( itBestPrice ).base() ); // erase in reverse iterator
                bid_map_->erase( bestPrice );
            }
            else
                ++itBestPrice;
        }
    }
}

inline
void OrderBook::Match(OrderTreeIt it, const Order* order, int& qty_to_match,
                      OrderTree* quotes, int best_price) {
    bool isBuy = order->Is_Buy();

    while(it != quotes->end() &&
            order != NULL &&
            IsMarketable( order, best_price, isBuy ) &&
            qty_to_match > 0) {
        Order* quote = *it;
        quotes->erase( it++ );

        int curQty = quote->GetQuantity();
        int execQty = std::min(curQty, qty_to_match);
        //bookTrade( execQty, buyer, seller );
        qty_to_match -= execQty;

        // add residual back to order tree
        if(curQty > execQty ) {
            //quote->GetQuantity() = curQty - execQty;
            quotes->emplace( quote );
        }
        else
            delete quote;
    }
    // order depletes current quote
    if( qty_to_match == 0 )
        delete order;
}

/**
 * NonMarketable order handling:
 *  Add liquidity to the same side of the book given and order
 *  time: O(1) if price level exists, O(logM) otherwise.
 *        Assume M is the avg number of quotes in the order book
 * */
inline
void OrderBook::Add(Order* order) {
    bool isBuy = order->Is_Buy();
    int price = order->GetPrice();
    PriceTree* priceTree = isBuy ? bid_tree_ : ask_tree_;
    PriceToNodeMap* priceToNodeMap = isBuy ? bid_map_ : ask_map_;

    PriceToNodeMapIt it = priceToNodeMap->find( price );
    if(it != priceToNodeMap->end()) {
        OrderTree* quotes = it->second->GetOrderTree();
        quotes->emplace(order);
    }
    else {
        PriceNode* priceNode = new PriceNode(price);
        priceNode->InsertOrder(order);
        priceTree->emplace(price, priceNode);
        priceToNodeMap->emplace(price, priceNode);
    }
    order->OnSubmitted();
}

inline
void OrderBook::Cancel(Order *order) {
    bool found = false;
    if(order->Is_Buy()) {
        PriceToNodeMapIt bidIt;
        FindOnMarket(order, bidIt);
        if(bidIt != bid_map_->end()) {
            bid_map_->erase(bidIt);
            found = true;
        }
    }
    else {
        PriceToNodeMapIt askIt;
        FindOnMarket(order, askIt);
        if(askIt != ask_map_->end()) {
            ask_map_->erase(askIt);
            found = true;
        }
    }
    if(found)
        order->OnCancelled();
    else
        order->OnCancelRejeceted("Order Not Found.");
}

inline
bool OrderBook::FindOnMarket(Order *order, PriceToNodeMapIt& it) {
    PriceToNodeMap* sidemp = order->Is_Buy() ? bid_map_ : ask_map_;
    for(it = sidemp->find(order->GetPrice()); it != sidemp->end(); ++it) {
        OrderTree* quotes = it->second->GetOrderTree();
        if(quotes->count(order))
            return true;
    }
    return false;
}

}


#endif /* Orderbook_h */
