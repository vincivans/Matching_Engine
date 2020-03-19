//
//  Order.cpp
//  Matching_Engine
//
//  Created by SiranWang on 3/14/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#include "Order.h"
#include <sstream>

namespace Matching {

long Order::last_order_id_(0);

Order::Order(bool buy_side, int quantity, int price):
            buy_side_(buy_side), quantity_(quantity),
            price_(price), fill_quantity_(0),
            fill_cost_(0),
            order_id_(++last_order_id_){}

bool Order::Is_Buy() const              { return buy_side_; }
long Order::GetOrderId() const          { return order_id_; }
int  Order::GetPrice() const            { return price_; }
int  Order::GetQuantity() const         { return quantity_; }
int  Order::GetFillQuantity() const     { return fill_quantity_; }
int  Order::GetFillCost() const         { return fill_cost_; }

time_t Order::GetTimeStamp() const                        { return timestamp_; }
const  Order::History& Order::GetHistory() const          { return history_; }
const  Order::StateChange& Order::GetCurrentState() const { return history_.back(); }

int  Order::GetOpenQuantity() const {
    if(fill_quantity_ < quantity_)
        return quantity_ - fill_quantity_;
    return 0;
}

void Order::OnSubmitted() {
    history_.emplace_back(Submitted);
}

void Order::OnAccepted() {
    std::stringstream msg;
    msg << (Is_Buy() ? "BUY" : "SELL") << quantity_ << ' @' << price_;
    history_.emplace_back(Accepted, msg.str());
}

void Order::OnRejected(const char *reason) {
    history_.emplace_back(Rejected, reason);
}

void Order::OnFilled(int fill_qty, int fill_cost) {
    fill_quantity_ += fill_qty;
    fill_cost_ += fill_cost;
    if(!GetOpenQuantity()) {
        std::stringstream msg;
        msg << fill_qty << " for " << fill_cost;
        history_.emplace_back(Filled, msg.str());
    }
    else {
        OnReplace(quantity_-fill_qty, fill_cost);
        std::stringstream msg;
        msg << "Partially filled " << fill_qty << " for " << fill_cost;
        history_.emplace_back(PartialFilled, msg.str());
    }
}

void Order::OnReplace(int size, int price) {
    if(GetCurrentState().state_ == Accepted) {
        quantity_ = size;
        price_ = price;
    }
}

void Order::OnCancelRequested() {
    history_.emplace_back(CancelRequested);
}

void Order::OnCancelled() {
    if(history_.back().state_ != Filled || history_.back().state_!= PartialFilled) {
        quantity_ = 0;
        history_.emplace_back(Cancelled);
    }
}

void Order::OnCancelRejeceted(const char *reason) {
    history_.emplace_back(CancelRejected, reason);
}

std::ostream & operator << (std::ostream & out, const Order::StateChange & event)
{
    out << "{";
    switch(event.state_)
    {
    case Order::Submitted:
        out << "Submitted ";
        break;
    case Order::Rejected:
        out << "Rejected ";
        break;
    case Order::Accepted:
        out << "Accepted ";
        break;
    case Order::PartialFilled:
        out << "PartialFilled ";
        break;
    case Order::Filled:
        out << "Filled ";
        break;
    case Order::CancelRequested:
        out << "CancelRequested ";
        break;
    case Order::CancelRejected:
        out << "CancelRejected ";
        break;
    case Order::Cancelled:
        out << "Cancelled ";
        break;
    }
    out << event.description_;
    out << "}";
    return out;
}
    
std::ostream & operator << (std::ostream & out, const Order & order)
{
    out << "[#" << order.GetOrderId();
    out << ' ' << (order.Is_Buy() ? "BUY" : "SELL");
    out << ' ' << order.GetQuantity();
    out << " $" << order.GetPrice();

    auto onMarket = order.GetOpenQuantity();
    if(onMarket != 0) {
        out << " Open: " << onMarket;
    }

    auto filled = order.GetFillQuantity();
    if(filled != 0) {
        out << " Filled: " << filled;
    }

    auto cost = order.GetFillCost();
    if(cost != 0) {
        out << " Cost: " << cost;
    }

   out << ']';
   
   return out;
}


}

