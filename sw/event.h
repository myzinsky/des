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
    event(uint64_t timestamp, signalInterface *sig);
    uint64_t timestamp;
    signalInterface *sig;

    bool operator < (const event& e) const
    {
        return (timestamp < e.timestamp);
    }
};

}

#endif // EVENT_H
