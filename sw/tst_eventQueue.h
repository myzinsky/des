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
    eq.insertEvent(event(123u,0));
    event e = eq.getNextEvent();

    ASSERT_EQ(e.time, 123u);
}

TEST(eventQueueTests, eventQueueSortTest)
{
    eventQueue eq;
    eq.insertEvent(event(100u,0));
    eq.insertEvent(event(300u,0));
    eq.insertEvent(event(100u,0));
    eq.insertEvent(event(200u,0));
    event e;
    e = eq.getNextEvent();
    ASSERT_EQ(e.time,100u);
    e = eq.getNextEvent();
    ASSERT_EQ(e.time,100u);
    e = eq.getNextEvent();
    ASSERT_EQ(e.time,200u);
    e = eq.getNextEvent();
    ASSERT_EQ(e.time,300);
}

#endif // TST_EVENTQUEUE_H
