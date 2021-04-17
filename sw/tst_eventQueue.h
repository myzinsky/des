#ifndef TST_EVENTQUEUE_H
#define TST_EVENTQUEUE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "eventqueue.h"

using namespace testing;
using namespace des;

TEST(eventQueueTests, insertIntoEventQueue)
{
    eventQueue eq;
    eq.insertEvent(event(123u, nullptr));
    event e = eq.getNextEvent();

    ASSERT_EQ(e.timestamp, 123u);
}

TEST(eventQueueTests, eventQueueSortTest)
{
    eventQueue eq;
    eq.insertEvent(event(100u,nullptr));
    eq.insertEvent(event(300u,nullptr));
    eq.insertEvent(event(100u,nullptr));
    eq.insertEvent(event(200u,nullptr));
    event e;
    e = eq.getNextEvent();
    ASSERT_EQ(e.timestamp,100u);
    e = eq.getNextEvent();
    ASSERT_EQ(e.timestamp,100u);
    e = eq.getNextEvent();
    ASSERT_EQ(e.timestamp,200u);
    e = eq.getNextEvent();
    ASSERT_EQ(e.timestamp,300);
}

#endif // TST_EVENTQUEUE_H
