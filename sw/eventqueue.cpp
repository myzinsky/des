#include "eventqueue.h"

des::eventQueue::eventQueue()
{
}

void des::eventQueue::insertEvent(des::event e)
{
    auto upper = std::upper_bound(queue.begin(), queue.end(), e);
    queue.insert(upper, e);
}

des::event des::eventQueue::getNextEvent()
{
    auto result = queue.front();
    queue.erase(queue.begin());
    return result;
}

uint64_t des::eventQueue::size()
{
    return queue.size();
}

uint64_t des::eventQueue::deltaSize(uint64_t currentTime)
{
    return std::count_if(queue.begin(), queue.end(), [&](event e){
        return e.timestamp == currentTime;
    });
}

uint64_t des::eventQueue::smallestTime()
{
    return std::min_element(queue.begin(), queue.end())->timestamp;
}
