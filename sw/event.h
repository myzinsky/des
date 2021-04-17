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
    event(uint64_t timestamp);
    event(uint64_t timestamp, std::experimental::coroutine_handle<> handle);
    uint64_t timestamp;
    std::experimental::coroutine_handle<> handle;

    bool operator < (const event& e) const
    {
        return (timestamp < e.timestamp);
    }
};

}

#endif // EVENT_H
