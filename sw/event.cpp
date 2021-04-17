#include "event.h"

des::event::event()
{
    timestamp = 0;
    sig = nullptr;
}

des::event::event(uint64_t timestamp, signalInterface *sig)
{
    this->timestamp = timestamp;
    this->sig = sig;
}
