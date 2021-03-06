/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef LOCKGUARD_TESTS_HPP
#define LOCKGUARD_TESTS_HPP

#include "riot/mutex.hpp"
#include "../testlock.hpp"

auto testFunc(TestLock& l)
{
    riot::LockGuard<TestLock> g(l);
}

// Expected Behavior: Guard locks Lock on Entry and Unlocks on Exit.
// -> TestLocks internal Counter must be two after testFunc call.
auto lockGuardTest(size_t& succeededTests, size_t& failedTests) -> void
{
    // Setup
    TestLock l;
    testFunc(l);

    // Check result
    if (l.timesLocked != 1 || l.timesUnlocked != 1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (l.timesLocked != 1 || l.timesUnlocked != 1)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all Lock specific tests
auto runLockGuardTests(size_t& succeededTests, size_t& failedTests) -> void
{
    lockGuardTest(succeededTests, failedTests);
}


#endif // LOCKGUARD_TESTS_HPP
