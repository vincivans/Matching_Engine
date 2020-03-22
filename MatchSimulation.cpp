//
//  MatchSimulation.cpp
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#include "MatchSimulation.h"

namespace Matching {

//Check is the input is a valid digit
bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}


void MatchSimulation::Run() {
    MarketEvents* me = new MarketEvents();
    OrderBook* book = new OrderBook(me);
    std::cout << "Welcome to the Matching Enginge, Please input the orders: \n" <<
                 "Add format: \n" <<
                 "msgtype(0), orderid, side(0 for Buy, 1 for Sell), quantity, price\n" <<
                 "Cancel format: \n" <<
                 "msgtype(1), orderid\n";
    
    std::string input;
    while(std::getline(std::cin, input)) {
    std::vector<std::string> words;
    split(input, " ,", words);
    if(!words.empty()) {
        int msg = stoi(words[0]);
        long orderid = 0;
        int quantity = 0, price = 0;
        Side side;
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
                    side = words[2]=="0" ? Buy : Sell;
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
                if(!is_number(words[4])) {
                    std::cerr << "Incorrect price\n";
                    break;
                }
                else {
                    price = stoi(words[4]);
                    if(price==0 || quantity>10000000) {
                        std::cerr << "Price should be in range [0, 10000000]\n";
                        break;
                    }
                }
                book->Enter(orderid, side, price, quantity);
                break;
            case 1:
                if(!is_number(words[1])) {
                    std::cerr << "Incorrect order_id\n";
                    break;
                }
                else
                    orderid = stol(words[1]);
                book->Cancel(orderid);
                break;
            default:
                std::cerr << "Unknown Message types!\n";
            }
        }
    }
}


}
