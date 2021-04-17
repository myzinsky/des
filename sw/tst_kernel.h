#ifndef TST_KERNEL_H
#define TST_KERNEL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "kernel.h"
#include "signal.h"

using namespace testing;
using namespace des;

TEST(kernelTests, sensitivity)
{
    int foo = 0;
    des::signal<bool> a("a",true);

    des::kernel::getInstance().registerProcess([&](){
        std::cout << "PROCESS" << std::endl;
        foo++;
    },{&a});

    // Testbench:
    des::kernel::getInstance().registerTestbench([&]() -> coroutine{
        des::kernel::getInstance().wait(10);
        co_await std::experimental::suspend_always{};
        std::cout << "TB" << std::endl;
        a.write(false);
        co_return 0;
    });

    des::kernel::getInstance().startSimulation();
    ASSERT_EQ(foo, 2);
}

TEST(kernelTests, rsLatch)
{
    des::kernel::getInstance().reset();

    des::signal<bool> s("s",true);
    des::signal<bool> r("r",false);
    des::signal<bool> q("q",true);
    des::signal<bool> n("n",false);

    // Process:
    des::kernel::getInstance().registerProcess([&](){
        q.write(!(r.read() || n.read()));
        n.write(!(s.read() || q.read()));
        std::cout << "PROCESS" << std::endl;
    },{&r,&s,&q,&n});

    // Testbench:
    des::kernel::getInstance().registerTestbench([&]() -> coroutine{
        des::kernel::getInstance().wait(10);
        co_await std::experimental::suspend_always{};
        s.write(false);
        r.write(true);
        des::kernel::getInstance().wait(10);
        co_await std::experimental::suspend_always{};
        s.write(false);
        r.write(false);
        co_return 0;
    });

    des::kernel::getInstance().startSimulation();

    ASSERT_EQ(q.read(), false);
    ASSERT_EQ(n.read(), true);
}

#endif // TST_KERNEL_H
