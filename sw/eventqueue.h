#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include<algorithm>
#include<iostream>
#include<vector>

#include "event.h"

namespace des {

class eventQueue
{
public:
    eventQueue();
    void insertEvent(event e);
    event getNextEvent();
    uint64_t size();
private:
    std::vector<event> queue;
};

}

#endif // EVENTQUEUE_H
