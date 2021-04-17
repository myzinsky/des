#ifndef KERNEL_H
#define KERNEL_H

#include <cstdint>
#include <functional>
#include <map>
#include <vector>

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

    void registerProcess(std::function<void()> function,
                         std::vector<signalInterface*> sensitivity);
    void registerTestbench(std::function<void()> function);
    void updateRequest(signalInterface *sig);
    void wait(u_int64_t time);
    void startSimulation();
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

    void debugSignals();

    std::map<signalInterface*, std::vector<std::function<void()>>> lut;
    std::vector<std::function<void()>> processes;
    std::function<void()> testbench;
};

}

#endif // KERNEL_H
