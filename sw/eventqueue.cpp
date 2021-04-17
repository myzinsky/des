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
