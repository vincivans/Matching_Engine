//
//  TestUtils.h
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef TestUtils_h
#define TestUtils_h

#include "../../src/OrderBook.h"
#include "../../src/MarketEvents.h"
#include "../../src/MarketEvents.cpp"

namespace Matching {

inline
bool EventsEqual(const std::vector<Event*>& lhs, const std::vector<Event*>& rhs) {
    if(lhs.size() != rhs.size()) return false;
    for(int i = 0; i < lhs.size(); ++i) {
        if(!lhs[i]->EqualTo(rhs[i]))
            return false;
    }
    return true;
}


}


#endif /* TestUtils_h */
