#include "event.h"

des::event::event()
{
    this->timestamp = 0;
    this->handle = nullptr;
}

des::event::event(uint64_t timestamp)
{
    this->timestamp = timestamp;
    this->handle = nullptr;
}

des::event::event(uint64_t timestamp, std::experimental::coroutine_handle<> handle)
{
    this->timestamp = timestamp;
    this->handle = handle;
}
