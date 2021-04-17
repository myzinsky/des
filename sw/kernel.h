#ifndef KERNEL_H
#define KERNEL_H

#include <cstdint>
#include <functional>
#include <map>
#include <vector>
#include <list>
#include <experimental/coroutine>

#include "eventqueue.h"

namespace des {

// Forward declaration instead of include eventQueue
class signalInterface;
class event;

class kernel
{
public:
    static kernel& getInstance()
    {
        static kernel instance;
        return instance;
    }

    struct awaitable {
        uint64_t time;
        bool await_ready() { return false; }
        void await_suspend(std::experimental::coroutine_handle<> handle) {
            kernel::getInstance().registerWait(time, handle);
        }
        void await_resume() {}
    };

    void registerAtomicProcess(std::function<void()> function, std::vector<signalInterface*> sensitivity);
    void registerSuspendableProcess(std::function<coroutine()> function);
    void updateRequest(signalInterface *sig);
    awaitable wait(uint64_t time);
    void startSimulation();
    void reset();
    uint64_t time();

private:
    eventQueue queue;
    uint64_t simulationTime;
    uint64_t nextTime;
    uint64_t delta;

    kernel (){
        simulationTime = 0;
        nextTime = 0;
    };

    void debugSignals(std::string msg);
    void registerWait(uint64_t time, std::experimental::coroutine_handle<> handle);

    std::map<signalInterface*, std::vector<std::function<void()>>> lut;
    std::list<des::signalInterface*> updateRequests;
    std::vector<std::function<void()>> processes;
    std::list<std::function<void()>> markedProcesses;
    std::function<coroutine()> testbench;
};

}

#endif // KERNEL_H
