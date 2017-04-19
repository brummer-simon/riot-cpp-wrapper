/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef THREAD_TESTS_HPP
#define THREAD_TESTS_HPP

#include "riot/thread.hpp"

using namespace riot;

struct TestContext
{
    int input;
    int output;
};

auto task(Thread<1, TestContext> & thread) -> void {
    printf("PID: %d, Name: %s, Status: %d\n", thread.getPid(), thread.getName(), thread.getStatus());
}

auto threadTestDefaultConstructor(size_t & succeededTests, size_t & failedTests) -> void
{

    Thread<1, TestContext> thread(task, thread::priority::Main - 1);

    (void) failedTests;

    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all Ringbuffer Tests
auto runThreadTests(size_t & succeededTests, size_t & failedTests) -> void
{
    threadTestDefaultConstructor(succeededTests, failedTests);
}

#endif // THREAD_TESTS_HPP
