#include "event.h"

des::event::event()
{
    this->time = 0;
    this->handle = nullptr;
}

des::event::event(uint64_t time)
{
    this->time = time;
    this->handle = nullptr;
}

des::event::event(uint64_t time, std::experimental::coroutine_handle<> handle)
{
    this->time = time;
    this->handle = handle;
}
