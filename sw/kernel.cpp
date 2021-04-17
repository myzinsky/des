#include "kernel.h"
#include "event.h"
#include <list>
#include "signal.h"

void des::kernel::registerProcess(std::function<void ()> function, std::vector<des::signalInterface*> sensitivity)
{
    for(auto sig : sensitivity) {
        lut[sig].push_back(function);
    }

    processes.push_back(function);
}

void des::kernel::registerTestbench(std::function<void ()> function)
{

}

void des::kernel::updateRequest(des::signalInterface *sig)
{
    des::event e(nextTime, sig); // dynamic creation and delete
    queue.insertEvent(e);
}

void des::kernel::wait(u_int64_t time)
{
    nextTime += time;
}

void des::kernel::startSimulation()
{
    std::list<des::signalInterface*> signals;

    // Kickstart Processes:
    for(auto process : processes) {
        process();
    }

    delta = 0;

    // Main Simulation Loop:
    while (true) {

        std::cout << "Queue: " << queue.size() << std::endl;

        // Stop:
        if(queue.size() == 0) {
            std::cout<< "End of Simulation" << std::endl;
            break;
        }

        // Advance Time:
        if(queue.deltaSize(simulationTime) == 0) {
            simulationTime = queue.smallestTime();
            delta = 0;
        }

        // Execute:
        for(uint64_t i = 0; i < queue.deltaSize(simulationTime); i++) {
            event e = queue.getNextEvent();
            signals.push_back(e.sig);
            signals.unique();

            for(auto function : lut[e.sig]) {
                function();
            }

            delta++;
            debugSignals();
        }

        // Update:
        for(auto * signal : signals) {
            signal->update();
        }

        debugSignals();
    }
}

uint64_t des::kernel::time()
{
    return simulationTime;
}

void des::kernel::debugSignals()
{
    std::cout << "@" << simulationTime << "+" << delta << "\t";
    for(auto l : lut)
    {
        signalInterface *sig = l.first;
        std::cout << sig->getName() << "=" << sig->toString() << " ";
    }
    std::cout << std::endl;
}
