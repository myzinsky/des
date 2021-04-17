#ifndef TST_KERNEL_H
#define TST_KERNEL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "kernel.h"
#include "signal.h"

using namespace testing;
using namespace des;

TEST(kernelTests, rsLatch)
{
    des::signal<bool> s("s",true);
    des::signal<bool> r("r",false);
    des::signal<bool> q("q",true);
    des::signal<bool> n("n",false);

    // Process:
    des::kernel::getInstance().registerProcess([&](){
        std::cout << "PROCESS 1 RUN: " << std::endl;
        q.write(!(r.read() || n.read()));
        n.write(!(s.read() || q.read()));
    },{&r,&s,&q,&n});

    // Testbench:
    des::kernel::getInstance().registerTestbench([&](){
        std::cout << "Testbench RUN:" << std::endl;
        s.write(false);
        r.write(true);
        des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(false);
    });


    des::kernel::getInstance().startSimulation();

    //ASSERT_EQ(q.read(), false);
    //ASSERT_EQ(n.read(), true);
}

#endif // TST_KERNEL_H
