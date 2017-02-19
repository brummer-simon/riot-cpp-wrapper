/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef LOCK_TESTS_HPP
#define LOCK_TESTS_HPP

#include "lockinterface_tests.hpp"
#include "lockguard_tests.hpp"

// Run all Lock specific tests
auto runLockTests(size_t& succeededTests, size_t& failedTests) -> void
{
    runLockInterfaceTests(succeededTests, failedTests);
    runLockGuardTests(succeededTests, failedTests);
}

#endif // LOCK_TESTS_HPP
