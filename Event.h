//
//  Event.h
//  Matching_Engine
//
//  Created by SiranWang on 3/17/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Event_h
#define Event_h

namespace Matching {

class Event {
public:
    Event(unsigned int t) : time_(t) {}
    
    virtual void ProcessEvent() = 0;
    
    const unsigned int time_;
};

}


#endif /* Event_h */
