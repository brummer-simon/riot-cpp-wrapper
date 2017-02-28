/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef LOCKINTERFACE_TESTS_HPP
#define LOCKINTERFACE_TESTS_HPP

#include "riot/mutex.hpp"
#include "riot/mutex/lockdummy_impl.hpp"

// This Test checks if the supplied LockDummy complies to LockGuard interface.
// This test can't fail but the tests can not compile...
auto lockInterfaceTestLockDummy(size_t& succeededTests, size_t& failedTests) -> void
{
    (void) failedTests;

    riot::keepout::LockDummy l;
    riot::LockGuard<riot::keepout::LockDummy> g(l);

    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// This Test checks if the supplied Mutex complies to LockGuard interface.
// This test can't fail but the tests can not compile...
auto lockInterfaceTestMutex(size_t& succeededTests, size_t& failedTests) -> void
{
    (void) failedTests;

    riot::Mutex l;
    riot::LockGuard<riot::Mutex> g(l);

    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all LockInterface tests
auto runLockInterfaceTests(size_t& succeededTests, size_t& failedTests) -> void
{
    lockInterfaceTestLockDummy(succeededTests, failedTests);
    lockInterfaceTestMutex(succeededTests, failedTests);
}

#endif // LOCKINTERFACE_TESTS_HPP
