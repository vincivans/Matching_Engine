//
//  MatchSimulation.hpp
//  Matching_Simulation
//
//  Created by SiranWang on 3/21/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef MatchSimulation_hpp
#define MatchSimulation_hpp

#include <string>
#include "OrderBook.h"

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

class MatchSimulation {
public:
    MatchSimulation() {}
    
    void Run();
};

}

#endif /* MatchSimulation_hpp */
