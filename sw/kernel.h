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

    void registerAtomicProcess(std::function<void()> function, std::vector<signalInterface*> sensitivity);
    void registerSuspendableProcess(std::function<coroutine()> function);
    void updateRequest(signalInterface *sig);
    std::experimental::suspend_always wait(u_int64_t time);
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

    std::map<signalInterface*, std::vector<std::function<void()>>> lut;
    std::list<des::signalInterface*> updateRequests;
    std::vector<std::function<void()>> processes;
    std::list<std::function<void()>> markedProcesses;
    std::function<coroutine()> testbench;
};

}

#endif // KERNEL_H
