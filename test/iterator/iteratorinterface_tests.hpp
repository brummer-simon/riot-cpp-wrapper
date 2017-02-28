/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef ITERATORINTERFACE_TESTS_HPP
#define ITERATORINTERFACE_TESTS_HPP

#include "riot/iterator.hpp"

// This Test checks if the supplied SequenceIterator complies to
// ReverseIterators interface. This test can't fail but the tests can not compile...
auto iteratorInterfaceTestSequenceIterator(size_t& succeededTests, size_t& failedTests) -> void
{
    (void) failedTests;
    int testSet[3] = {0,1,2};

    riot::SequenceIterator<int> sIt(testSet);
    riot::BackwardIterator<riot::SequenceIterator<int> > bIt(sIt);

    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all LockInterface tests
auto runIteratorInterfaceTests(size_t& succeededTests, size_t& failedTests) -> void
{
    iteratorInterfaceTestSequenceIterator(succeededTests, failedTests);
}

#endif // ITERATORINTERFACE_TESTS_HPP
