#include "kernel.h"
#include "event.h"
#include <list>
#include "signal.h"

void des::kernel::registerProcess(std::function<void()> function, std::vector<des::signalInterface*> sensitivity)
{
    for(auto sig : sensitivity) {
        lut[sig].push_back(function);
    }

    processes.push_back(function);
}

void des::kernel::updateRequest(des::signalInterface *sig)
{
    updateRequests.push_back(sig);
    updateRequests.unique();
}

des::kernel::awaitable des::kernel::wait(uint64_t time)
{
    return awaitable{time};
}

void des::kernel::registerWait(uint64_t time, std::experimental::coroutine_handle<> handle)
{
    event e(simulationTime+time, handle);
    queue.insertEvent(e);
}

void des::kernel::startSimulation(uint64_t time)
{
    stopTime = time;
    stop = true;
    startSimulation();
}

void des::kernel::startSimulation()
{
    //debugSignals("Start Simulation");

    // Initialize: Kickstart Processes
    for(auto process : processes) {
        process();
    }

    //debugSignals("Initialize");

    // Main Simulation Loop:
    while (true) {

        if(markedProcesses.empty()) {
            // Advance Time:
            if(queue.size() != 0) {
                event e = queue.getNextEvent();
                simulationTime = e.time;
                delta = 0;

                if(stop && (simulationTime >= stopTime)) {
                    // Stop Simulation:
                    //debugSignals("End of Simulation");
                    reset();
                    break;
                }

                e.handle.resume(); // if(!e.handle.done()) { ... }
                //debugSignals("Advanced Time");
                if(vcdActive) {
                    vcdSignals();
                }
            } else {
                // Stop Simulation:
                //debugSignals("End of Simulation");
                reset();
                break;
            }
        } else {

            // Evaluate: Execute all processes which are marked for execution
            for(auto process : markedProcesses) {
                process();
                //debugSignals("Evaluate");
                if(++delta == 1000) {
                    // Stop Simulation:
                    std::cout << "Delta Limit Reached (1000)" << std::endl;
                    reset();
                    break;
                }
            }
            markedProcesses.clear();
        }

        // Update:
        for(auto * signal : updateRequests) {
            signal->update();
            for(auto &process : lut[signal]) {
                markedProcesses.push_back(process);
                //markedProcesses.unique(); // TODO?
            }
        }
        updateRequests.clear();
        //debugSignals("Update Done");
    }
}

void des::kernel::reset()
{
    if(vcdActive) {
        vcdSave();
    }
    simulationTime = 0;
    delta = 0;
    stop = false;
    stopTime = 0;
    lut.clear();
    updateRequests.clear();
    processes.clear();
    markedProcesses.clear();
    testbench = nullptr;
    vcd = "";
    vcdLut.clear();
    vcdActive = false;
}

uint64_t des::kernel::time()
{
    return simulationTime;
}

void des::kernel::debugSignals(std::string msg)
{
    if(lut.size() != 0) {
        std::cout << "@" << simulationTime << "+" << delta << "\t";
        for(auto &l : lut)
        {
            signalInterface *sig = l.first;
            std::cout << sig->getName() << "=" << sig->toString() << " ";
        }
        std::cout << ": " << msg;
        std::cout << std::endl;
    }
}

void des::kernel::vcdInit(std::string file)
{
    vcdActive = true;
    vcdFile = file;
    vcd += "$timescale 1ns $end\n";
    vcd += "$scope module logic $end\n";
    char c = 'a';
    for(auto &l : lut)
    {
        signalInterface *sig = l.first;
        vcd += "$var wire 1 " + std::string(1,c) + " " + sig->getName() + " $end\n";
        vcdLut[sig] = c;
        c += 1;
    }
    vcd += "$upscope $end\n";
    vcd += "$enddefinitions $end\n";
}

void des::kernel::vcdSignals()
{
    vcd += "#" + std::to_string(simulationTime) + "\n";

    for(auto &l : lut)
    {
        signalInterface *sig = l.first;
        vcd += sig->toString() + vcdLut[sig] + "\n";
    }
}

void des::kernel::vcdSave()
{
    std::ofstream out(vcdFile+".vcd");
    out << vcd;
    out.close();
}
