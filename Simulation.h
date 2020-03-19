//
//  Simulation.h
//  Matching_Engine
//
//  Created by SiranWang on 3/17/20.
//  Copyright © 2020 SiranWang. All rights reserved.
//

#ifndef Simulation_h
#define Simulation_h

#include <queue>
#include <vector>
#include "Event.h"

namespace Matching {

struct EventComparator {
    bool operator() (const Event* lhs, const Event* rhs) {
        return lhs->time_ > rhs->time_;
    }
};

class Simulation {
public:
    Simulation() : time_(0) {}
    void Run();
    void ScheduleEvent(Event* new_event) { event_queue_.push(new_event); }
    
    unsigned int time_;
protected:
    std::priority_queue<Event*,
                        std::vector<Event*>,
                        EventComparator> event_queue_;
};

inline
void Simulation::Run() {
    while(!event_queue_.empty()) {
        Event* nxt_event = event_queue_.top();
        event_queue_.pop();
        time_ = nxt_event->time_;
        nxt_event->ProcessEvent();
        delete nxt_event;
    }
}

}

#endif /* Simulation_h */
