//
//  MatchingEngine.cpp
//  Matching_Engine
//
//  Created by SiranWang on 3/14/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#include <sstream>
#include <vector>
#include "MatchingEngine.h"

namespace Matching {

// Parse a string into tokens breaking on delimiters.
// @param input The string to parse.
// @param delimiter A set of delimiter characters.
// @param[out] tokens  the tokens parsed from the string.
template<typename INPUT_STRING,typename DELIMITER_STRING,typename STRING_CONTAINER>
void split(const INPUT_STRING & input, const DELIMITER_STRING & delimiter,
           STRING_CONTAINER & tokens) {
    size_t pos = 0;
    size_t end = input.length();
    while(pos < end) {
        auto last = input.find_first_of(delimiter, pos);
        if(last == std::string::npos) {
            last = end;
        }
        tokens.push_back(input.substr(pos, last - pos));
        pos = ++last;
    }
}


//Check is the input is a valid digit
bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void MatchingEngine::AddOrder(Order* order) {
     order_book_->Add(order);
}


void MatchingEngine::CancelOrder(Order* order) {
    order_book_->Cancel(order);
}


void MatchingEngine::ProcessOrder() {
    std::string input;
    while(std::getline(std::cin, input)) {
    std::vector<std::string> words;
    split(input, " ,", words);
    if(!words.empty()) {
        int msg = stoi(words[0]);
        long orderid = 0;
        int quantity = 0, price = 0;
        bool side = 0;
        Order* order;
        switch (msg) {
            case 0:
                if(!is_number(words[1])) {
                    std::cerr << "Incorrect order_id\n";
                    break;
                }
                else
                    orderid = stol(words[1]);
                if(!is_number(words[2])) {
                    std::cerr << "Incorrect side\n";
                    break;
                }
                else
                    side = words[2]=="0" ? 0 : 1;
                if(!is_number(words[3])) {
                    std::cerr << "Incorrect quantity\n";
                    break;
                }
                else {
                    quantity = stoi(words[3]);
                    if(quantity==0 || quantity>1000000000) {
                        std::cerr << "Quantity should be in range [0, 1000000000]\n";
                        break;
                    }
                }
                if(!is_number(words[4]))
                    std::cerr << "Incorrect price\n";
                else {
                    price = stoi(words[4]);
                    if(price==0 || quantity>10000000) {
                        std::cerr << "Price should be in range [0, 10000000]\n";
                        break;
                    }
                }
                order = new Order(side, quantity, price);
                orders_[order->GetOrderId()] = order;
                AddOrder(order);
                break;
            case 1:
                if(!is_number(words[1])) {
                    std::cerr << "Incorrect order_id\n";
                    break;
                }
                else
                    orderid = stol(words[1]);
                if(!orders_.count(orderid)) {
                    std::cerr << "Order not found\n";
                    break;
                }
                CancelOrder(orders_[orderid]);
                break;
            default:
                std::cerr << "Unknown Message types!\n";
        }
    }
    }
}

    


/*
MatchingEngine::MatchingEngine() {
    order_book_ = new OrderBook();
}

void MatchingEngine::ProcessOrder(Order *order) {
    int qtyToMatch = order->GetQuantity();
    
    order_book_->Match(order, qtyToMatch);
    
    // post non marketable portion
    if(qtyToMatch > 0 ) {
        //if(qtyToMatch != order->GetQuantity())
            //order->GetQuantity() = qtyToMatch;
        order_book_->Add(order);
    }
}

void MatchingEngine::CancelOrder(long order_id) {
    //order_book_->Cancel(order);
}

int MatchingEngine::Run(std::ostream & out) {
    std::string input;
    out << "Please select the action you want:"
        << std::endl;
    
    out << "Add(0): Create a new order and add it to the book\n"
        << "  Arguments:\n"
        << "     <Side> BUY(0) or SELL(1),<Quantity>,<Price> \n"
        << "     ;              end of order\n"
        << std::endl;

    out << "Cancel(1): Request cancel an existing order\n"
        << "  Arguments:\n"
        << "     <order#>\n"
        << "     ;              end of cancel request (optional)\n"
        << std::endl;
    
    while(std::getline(std::cin, input)) {
        std::vector<std::string> words;
        split(input, " ,", words);
        if(!words.empty()) {
            int msg = stoi(words[0]);
            long orderid = 0;
            int quantity = 0, price = 0;
            bool side = 0;
            Order* order;
            switch (msg) {
                case 0:
                    if(!is_number(words[1])) {
                        std::cerr << "Incorrect order_id\n";
                        break;
                    }
                    else
                        orderid = stol(words[1]);
                    if(!is_number(words[2])) {
                        std::cerr << "Incorrect side\n";
                        break;
                    }
                    else
                        side = words[2]=="0" ? 0 : 1;
                    if(!is_number(words[3])) {
                        std::cerr << "Incorrect quantity\n";
                        break;
                    }
                    else {
                        quantity = stoi(words[3]);
                        if(quantity==0 || quantity>1000000000) {
                            std::cerr << "Quantity should be in range [0, 1000000000]\n";
                            break;
                        }
                    }
                    if(!is_number(words[4]))
                        std::cerr << "Incorrect price\n";
                    else {
                        price = stoi(words[4]);
                        if(price==0 || quantity>10000000) {
                            std::cerr << "Price should be in range [0, 10000000]\n";
                            break;
                        }
                    }
                    order = new Order(side, quantity, price);
                    ProcessOrder(order);
                    break;
                case 1:
                    if(!is_number(words[1])) {
                        std::cerr << "Incorrect order_id\n";
                        break;
                    }
                    else
                        orderid = stol(words[1]);
                    CancelOrder(orderid);
                    break;
                default:
                    std::cerr << "Unknown Message types!\n";
            }
        }
    }
    return 0;
}*/



}
