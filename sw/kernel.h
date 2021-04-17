#ifndef KERNEL_H
#define KERNEL_H

#include <cstdint>
#include <functional>
#include <map>
#include <vector>
#include <list>
#include <experimental/coroutine>

#include <string>
#include <fstream>
#include <iostream>

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

    void registerProcess(std::function<void()> function, std::vector<signalInterface*> sensitivity = {});
    void updateRequest(signalInterface *sig);
    awaitable wait(uint64_t time);
    void startSimulation();
    void startSimulation(uint64_t time);
    void reset();
    uint64_t time();

    // VCD Stuff:
    void vcdInit(std::string file);
    void vcdSignals();
    void vcdSave();
    bool vcdActive;
    std::string vcd;
    std::string vcdFile;
    std::map<signalInterface*, char> vcdLut;

private:
    eventQueue queue;
    uint64_t simulationTime;
    uint64_t delta;
    bool stop;
    uint64_t stopTime;

    kernel (){
        reset();
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
