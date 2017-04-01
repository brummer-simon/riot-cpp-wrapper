/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef TESTS_HPP
#define TESTS_HPP

#include "mutex/lock_tests.hpp"
#include "iterator/iterator_tests.hpp"
#include "array/array_tests.hpp"
#include "ringbuffer/ringbuffer_tests.hpp"
#include "ringbuffer/lockedringbuffer_tests.hpp"
#include "ringbuffer/blockingringbuffer_tests.hpp"
#include "semaphore/semaphore_tests.hpp"

// Run all Tests.
auto runAllTests() -> void
{
    size_t succeededTests = 0;
    size_t failedTests = 0;

    printf("\n--- Testrun started ---\n\n");

    runLockTests(succeededTests, failedTests);
    runIteratorTests(succeededTests, failedTests);
    runArrayTests(succeededTests, failedTests);
    runRingbufferTests(succeededTests, failedTests);
    runLockedRingbufferTests(succeededTests, failedTests);
    runBlockingRingbufferTests(succeededTests, failedTests);
    runSemaphoreTests(succeededTests, failedTests);

    printf("\n--- Testrun finished ---\n\n");
    printf("Tests ran: %u\n", succeededTests + failedTests);
    printf("Succeeded: %u\n", succeededTests);
    printf("Failed: %u\n", failedTests);
}

#endif // TESTS_HPP
