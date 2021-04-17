#ifndef EVENT_H
#define EVENT_H

#include <cstdint>
#include <functional>


namespace des {

class signalInterface;

template<typename TYPE>
class signal;

class event
{
public:
    event();
    event(uint64_t timestamp);
    uint64_t timestamp;

    bool operator < (const event& e) const
    {
        return (timestamp < e.timestamp);
    }
};

}

#endif // EVENT_H
