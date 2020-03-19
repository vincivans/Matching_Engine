//
//  Order.h
//  Matching_Engine
//
//  Created by SiranWang on 3/14/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Order_h
#define Order_h

#include <string>
#include <vector>
#include "OrderFwd.h"

namespace Matching {

class Order {
public:
    enum State {
         Submitted,
         Rejected,//terminal state
         Accepted,
         PartialFilled,
         Filled,//terminal state
         CancelRequested,
         CancelRejected,
         Cancelled,//terminal state
    };
    
    struct StateChange {
        State state_;
        std::string description_;
        StateChange():state_(Submitted){}
        StateChange(State state, const std::string& description="") :
            state_(state), description_(description) {}
    };
    
    typedef std::vector<StateChange> History;

public:
    Order(bool buy_side, int quantity, int price);
    
    
    //Get method for private members
    bool Is_Buy() const;
    int GetPrice() const;
    int GetQuantity() const;
    long GetOrderId() const;
    int GetFillQuantity() const;
    int GetOpenQuantity() const;
    int GetFillCost() const;
    time_t GetTimeStamp() const;
    const History& GetHistory() const;
    const StateChange& GetCurrentState() const;
    
    
    //Order life cycle events
    void OnSubmitted();
    void OnAccepted();
    void OnRejected(const char* reason);
    
    void OnFilled(int fill_qty, int fill_cost);
    void OnReplace(int size_, int price);
    
    void OnCancelRequested();
    void OnCancelled();
    void OnCancelRejeceted(const char* reason);
public:
    const long order_id_;
private:
    time_t timestamp_;
    static long last_order_id_;
    bool buy_side_;
    int quantity_;
    int price_;
    
    int fill_quantity_;
    int fill_cost_;
    std::vector<StateChange> history_;
};

std::ostream & operator << (std::ostream & out, const Order& order);
std::ostream & operator << (std::ostream & out, const Order::StateChange & event);

}



#endif /* Order_h */
