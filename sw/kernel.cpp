#include "kernel.h"
#include "event.h"
#include <list>
#include "signal.h"

void des::kernel::registerProcess(std::function<void ()> function, std::vector<des::signalInterface*> sensitivity)
{
    if(!sensitivity.empty()) {
        for(auto sig : sensitivity) {
            lut[sig].push_back(function);
        }
    }

    processes.push_back(function);
}

void des::kernel::registerTestbench(std::function<coroutine ()> function)
{
    testbench = function;
}

void des::kernel::updateRequest(des::signalInterface *sig)
{
    updateRequests.push_back(sig);
    updateRequests.unique();
}

void des::kernel::wait(u_int64_t time)
{
    nextTime += time;
}

void des::kernel::startSimulation()
{
    debugSignals("Start Simulation");

    // Initialize: Kickstart Processes
    for(auto process : processes) {
        process();
    }

    coroutine tb = testbench();
    debugSignals("Initialize");

    delta = 0;

    // Main Simulation Loop:
    while (true) {

        if(markedProcesses.empty()) {
            // Advance Time:
            simulationTime = nextTime;
            debugSignals("Advanced Time");
            delta = 0;

            if(!tb.handle.done()) {
                tb.handle.resume();
            } else {
                // Stop Simulation:
                debugSignals("End of Simulation");
                break;
            }
        } else {

            // Evaluate: Execute all processes which are marked for execution
            for(auto process : markedProcesses) {
                process();
                delta++;
                debugSignals("Evaluate");
            }
            markedProcesses.clear();
        }

        // Update:
        for(auto * signal : updateRequests) {
            signal->update();
            for(auto process : lut[signal]) {
                markedProcesses.push_back(process);
                // TODO: markedProcesses.unique();
            }
        }
        updateRequests.clear();
        debugSignals("Update Done");
    }
}

void des::kernel::reset()
{
    simulationTime = 0;
    nextTime = 0;
    lut.clear();
    updateRequests.clear();
    processes.clear();
    markedProcesses.clear();
    testbench = nullptr;
}

uint64_t des::kernel::time()
{
    return simulationTime;
}

void des::kernel::debugSignals(std::string msg)
{
    if(lut.size() != 0) {
        std::cout << "@" << simulationTime << "+" << delta << "\t";
        for(auto l : lut)
        {
            signalInterface *sig = l.first;
            std::cout << sig->getName() << "=" << sig->toString() << " ";
        }
        std::cout << ": " << msg;
        std::cout << std::endl;
    }
}
