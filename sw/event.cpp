#include "event.h"

des::event::event()
{
    timestamp = 0;
}

des::event::event(uint64_t timestamp)
{
    this->timestamp = timestamp;
}
