#ifndef DESCOROUTINE_H
#define DESCOROUTINE_H

#include <experimental/coroutine>

namespace des {

struct coroutine{
    struct promise {
        coroutine get_return_object() noexcept {
            return std::experimental::coroutine_handle<promise>::from_promise(*this);
        }
        std::experimental::suspend_never initial_suspend() noexcept { return {}; }
        std::experimental::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() noexcept {}
    };

    using promise_type = promise;
    coroutine(std::experimental::coroutine_handle<promise> h) : handle(h) {}

    std::experimental::coroutine_handle<promise> handle;
};

}


#endif // DESCOROUTINE_H
