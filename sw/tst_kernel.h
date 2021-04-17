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
    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        std::cout << "TB" << std::endl;
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
        std::cout << "Process 1:" << des::kernel::getInstance().time() << std::endl;
        co_await des::kernel::getInstance().wait(40);
        std::cout << "Process 1:" << des::kernel::getInstance().time() << std::endl;
        foo.push_back(1);
    },{});

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(20);
        std::cout << "Process 2:" << des::kernel::getInstance().time() << std::endl;
        foo.push_back(2);
        co_await des::kernel::getInstance().wait(20);
        std::cout << "Process 2:" << des::kernel::getInstance().time() << std::endl;
        foo.push_back(2);
    },{});

    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(30);
        std::cout << "Process 3:" << des::kernel::getInstance().time() << std::endl;
        foo.push_back(3);
        co_await des::kernel::getInstance().wait(30);
        std::cout << "Process 3:" << des::kernel::getInstance().time() << std::endl;
        foo.push_back(3);
    },{});

    des::kernel::getInstance().startSimulation();

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
        std::cout << "PROCESS" << std::endl;
    },{&r,&s,&q,&n});

    // Testbench:
    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(true);
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(false);
    },{});

    des::kernel::getInstance().startSimulation();

    ASSERT_EQ(q.read(), false);
    ASSERT_EQ(n.read(), true);
}

TEST(kernelTests, rsLatch2)
{
    des::signal<bool> s("s",true);
    des::signal<bool> r("r",false);
    des::signal<bool> q("q",true);
    des::signal<bool> n("n",false);

    // Process:
    des::kernel::getInstance().registerProcess([&](){
        q.write(!(r.read() || n.read()));
        std::cout << "PROCESS 1" << std::endl;
    },{&r,&n});

    des::kernel::getInstance().registerProcess([&](){
        n.write(!(s.read() || q.read()));
        std::cout << "PROCESS 2" << std::endl;
    },{&s,&q});

    // Testbench:
    des::kernel::getInstance().registerProcess([&]() -> coroutine{
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(true);
        co_await des::kernel::getInstance().wait(10);
        s.write(false);
        r.write(false);
    },{});

    des::kernel::getInstance().startSimulation();

    ASSERT_EQ(q.read(), false);
    ASSERT_EQ(n.read(), true);
}

#endif // TST_KERNEL_H
