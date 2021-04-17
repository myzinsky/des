#ifndef EVENT_H
#define EVENT_H

#include <cstdint>
#include <functional>
#include "descoroutine.h"

namespace des {

class signalInterface;

template<typename TYPE>
class signal;

class event
{
public:
    event();
    event(uint64_t time);
    event(uint64_t time, std::experimental::coroutine_handle<> handle);
    uint64_t time;
    std::experimental::coroutine_handle<> handle;

    bool operator < (const event& e) const
    {
        return (time < e.time);
    }
};

}

#endif // EVENT_H
