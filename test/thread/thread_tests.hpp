/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef THREAD_TESTS_HPP
#define THREAD_TESTS_HPP

#include <msg.h>
#include "riot/thread.hpp"

using namespace riot;

thread::Stack<> testStack;

// Tests thread::Stack implementation. This Test creates over and underflows of the
// stack implementation and tests if the stack corruption detection mechanism works.
auto threadStackTest(size_t & succeededTests, size_t & failedTests) -> void
{
    thread::Stack<1, 0xA5> s;
    // Verify unused stack. Must work
    if (s.valid() != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: s.valid() != 0\n");
        failedTests += 1;
    }
    // Simulate overflow
    uint8_t * addr = s.getBase();
    addr[1] = -1;
    // Overflowed stack. Valid() return positive value.
    if (s.valid() > 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: s.valid() > 0\n");
        failedTests += 1;
    }
    // Fix Overflow, create underflow
    addr[1] = 0xA5;
    addr[-1] = -1;
    // Underflowed stack. Valid() return negative value.
    if (s.valid() < 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: s.valid() < 0\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

auto threadTestDefaultConstructor(size_t & succeededTests, size_t & failedTests) -> void
{
    // Init Context
    int val = 0;

    // Define Thread Task. Increase given int by one
    auto increase = [](Thread<int> & thread) -> void
    {
        thread.getContext() += 1;
    };

    // Execute Thread with given context.
    Thread<int> counter(increase, testStack, thread::priority::Main - 1, val);

    // Wait until Thread is done
    counter.join();

    // Evaluate result
    if (counter.getContext() != val + 1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all Ringbuffer Tests
auto runThreadTests(size_t & succeededTests, size_t & failedTests) -> void
{
    threadStackTest(succeededTests, failedTests);
    threadTestDefaultConstructor(succeededTests, failedTests);
}

#endif // THREAD_TESTS_HPP
