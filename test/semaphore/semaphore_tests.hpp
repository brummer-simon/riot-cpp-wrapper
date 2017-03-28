/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef SEMAPHORE_TESTS_HPP
#define SEMAPHORE_TESTS_HPP

#include <climits>
#include "riot/semaphore.hpp"

// Test Constructors: They should behave as Expected.
auto semaphoreTestDefaultConstructor(size_t& succeededTests, size_t& failedTests) -> void
{
    // Default Constructor
    riot::Semaphore s(0);
    (void) s;
    (void) failedTests;
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test post(): Expected behavoir: wait must not block if semaphore count
// is grater than 1 after post. On normal operation, zero is returned and -EOVERFLOW
// if the internal counter would overflow.
auto semaphoreTestPost(size_t& succeededTests, size_t& failedTests) -> void
{
    // Post successfull(following wait must not block)
    riot::Semaphore s1(0);
    int ret = s1.post();
    s1.wait();
    if (ret != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0)\n");
        failedTests += 1;
    }
    // Post overflow
    riot::Semaphore s2(UINT_MAX);
    ret = s2.post();
    if (ret != -EOVERFLOW) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -EOVERFLOW)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test wait(): Expected behavoir: wait must not block if semaphore count
// is greater than 1. On normal operation, zero is returned and -ECANCELED
// if the semaphore was destroyed.
auto semaphoreTestWait(size_t& succeededTests, size_t& failedTests) -> void
{
    // Wait non-block
    riot::Semaphore s(1);
    if (s.wait() != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0)\n");
        failedTests += 1;
    }
    // Destroy Semaphore. Call must be -ECANCELED
    s.~Semaphore();
    if (s.wait() != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (s.wait() != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test tryWait(): Expected behavoir: tryWait must never block.
// On normal operation, zero is returned if the semaphore was posted,
// -EAGAIN if the semaphore was not postend and -ECANCELED if the
// semaphore was destroyed.
auto semaphoreTestTryWait(size_t& succeededTests, size_t& failedTests) -> void
{
    // TryWait: Semaphore not posted ...
    riot::Semaphore s(0);
    if (s.tryWait() != -EAGAIN) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (s.tryWait() != -EAGAIN)\n");
        failedTests += 1;
    }
    // Post semaphore
    s.post();
    if (s.tryWait() != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (s.tryWait() != 0)\n");
        failedTests += 1;
    }
    // Destroy Semaphore. Call must be -ECANCELED
    s.~Semaphore();
    if (s.tryWait() != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (s.tryWait() != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test waitTimed(): Expected behavoir: timedWait must not block, if
// the semaphore was posted, and block until the timeout expires if not posted.
// On normal operation, zero is returned if the semaphore was posted,
// -ETIMEDOUT if the semaphore was not posted and the timeout expired and
// -ECANCELED if the semaphore was destroyed.
auto semaphoreTestWaitTimed(size_t& succeededTests, size_t& failedTests) -> void
{
    // TryWait: Semaphore not posted ...
    riot::Semaphore s(1);
    if (s.waitTimed(1000) != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (s.waitTimed() != 0)\n");
        failedTests += 1;
    }
    if (s.waitTimed(1000) != -ETIMEDOUT) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (s.waitTimed() != -ETIMEDOUT)\n");
        failedTests += 1;
    }
    // Destroy Semaphore. Call must be -ECANCELED
    s.~Semaphore();
    if (s.tryWait() != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (s.waitTimed() != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all Ringbuffer Tests
auto runSemaphoreTests(size_t& succeededTests, size_t& failedTests) -> void
{
    semaphoreTestDefaultConstructor(succeededTests, failedTests);
    semaphoreTestPost(succeededTests, failedTests);
    semaphoreTestWait(succeededTests, failedTests);
    semaphoreTestTryWait(succeededTests, failedTests);
    semaphoreTestWaitTimed(succeededTests, failedTests);
}

#endif // SEMAPHORE_TESTS_HPP
