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
        foo++;
    },{&a});

    // Testbench:
    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        a.write(false);
    },{});

    des::kernel::getInstance().startSimulation();
    ASSERT_EQ(foo, 2);
}

TEST(kernelTests, eventOrder) {

    std::vector<int> foo;
    std::vector<int> bar = {1,2,3,2,1,3};

    // 10 20 30 40 50 60
    //  1  2  3  2  1  3

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        foo.push_back(1);
        co_await des::kernel::getInstance().wait(40);
        foo.push_back(1);
    });

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(20);
        foo.push_back(2);
        co_await des::kernel::getInstance().wait(20);
        foo.push_back(2);
    });

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(30);
        foo.push_back(3);
        co_await des::kernel::getInstance().wait(30);
        foo.push_back(3);
    });

    des::kernel::getInstance().startSimulation();

    ASSERT_EQ(foo, bar);
}

TEST(kernelTests, eventOrder2) {

    std::vector<int> foo;
    std::vector<int> bar  = {1,1,2,2};
    std::vector<int> bar2 = {1,2,1,2};

    // 10 20 20 40
    //  1  2  1  2
    //  1  1  2  1

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        foo.push_back(1);
        co_await des::kernel::getInstance().wait(10);
        foo.push_back(1);
    });

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(20);
        foo.push_back(2);
        co_await des::kernel::getInstance().wait(20);
        foo.push_back(2);
    });

    des::kernel::getInstance().startSimulation();

    ASSERT_TRUE(foo == bar || foo == bar2);
}

TEST(kernelTests, stopSimulation) {

    std::vector<int> foo;
    std::vector<int> bar = {1,2};

    // 10 20
    //  1  2

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        foo.push_back(1);
        co_await des::kernel::getInstance().wait(20);
        foo.push_back(1);
    });

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(20);
        foo.push_back(2);
        co_await des::kernel::getInstance().wait(20);
        foo.push_back(2);
    });

    des::kernel::getInstance().startSimulation(25);

    ASSERT_EQ(foo, bar);
}

TEST(kernelTests, rsLatch)
{
    des::signal<bool> s("s",true);
    des::signal<bool> r("r",false);
    des::signal<bool> q("q",true);
    des::signal<bool> n("n",false);

    // Process:
    des::kernel::getInstance().registerProcess([&](){
        q.write(!(r.read() || n.read()));
        n.write(!(s.read() || q.read()));
    },{&r,&s,&q,&n});

    // Testbench:
    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(true);
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(false);
        co_await des::kernel::getInstance().wait(10);
    });

    des::kernel::getInstance().vcdInit("rslatch");
    des::kernel::getInstance().startSimulation();

    ASSERT_EQ(q.read(), false);
    ASSERT_EQ(n.read(), true);
}

TEST(kernelTests, rsLatchFeedbackLoop)
{
    des::signal<bool> s("s",true);
    des::signal<bool> r("r",false);
    des::signal<bool> q("q",true);
    des::signal<bool> n("n",false);

    int counter = 0;

    // Process:
    des::kernel::getInstance().registerProcess([&](){
        q.write(!(r.read() || n.read()));
        n.write(!(s.read() || q.read()));
        counter++;
    },{&r,&s,&q,&n});

    // Testbench:
    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        s.write(true);
        r.write(true);
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(false);
        co_await des::kernel::getInstance().wait(10);
    });

    des::kernel::getInstance().startSimulation();
    ASSERT_EQ(counter,1003);
}

TEST(kernelTests, rsLatch2)
{
    des::signal<bool> s("s",true);
    des::signal<bool> r("r",false);
    des::signal<bool> q("q",true);
    des::signal<bool> n("n",false);

    // Process:
    des::kernel::getInstance().registerProcess([&]() {
        q.write(!(r.read() || n.read()));
    },{&r,&n});

    des::kernel::getInstance().registerProcess([&]() {
        n.write(!(s.read() || q.read()));
    },{&s,&q});

    // Testbench:
    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(true);
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(false);
    });

    des::kernel::getInstance().startSimulation();

    ASSERT_EQ(q.read(), false);
    ASSERT_EQ(n.read(), true);
}

#endif // TST_KERNEL_H
