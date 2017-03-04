/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef BACKWARDITERATOR_TESTS_HPP
#define BACKWARDITERATOR_TESTS_HPP

#include "riot/iterator.hpp"

struct TestBackwardIteratorObj
{
    int cnt;
    TestBackwardIteratorObj() : cnt(0) {}
    auto increase() -> void {++cnt;}
};

// Test == Function: Expected Behavior: Two BackwardIterators pointing to the
// same location must be equivalent -> it1 == it2 must be equal to it2 == it1.
auto backwardIteratorTestEqualityFunction(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};

    riot::SequenceIterator<int> sIt1(testSet);
    riot::SequenceIterator<int> sIt2(testSet);
    riot::BackwardIterator<riot::SequenceIterator<int> > it1(sIt1);
    riot::BackwardIterator<riot::SequenceIterator<int> > it2(sIt2);
    if (!(it1 == it2)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (it1 != it2)\n");
        failedTests += 1;
        return;
    }
    if (!(it2 == it1)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (it2 != it1)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test != Function: Expected Behavior: Two BackwardIterators pointing to the
// different locations must be not equival -> it1 != it2 must be equal to it2 != it1.
auto backwardIteratorTestNotEqualityFunction(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> sIt1(testSet);
    riot::SequenceIterator<int> sIt2(testSet + 1);
    riot::BackwardIterator<riot::SequenceIterator<int> > it1(sIt1);
    riot::BackwardIterator<riot::SequenceIterator<int> > it2(sIt2);
    if (!(it1 != it2)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (it1 == it2)\n");
        failedTests += 1;
        return;
    }
    if (!(it2 != it1)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (it2 == it1)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test * Operator. Expected Behavoir: After derefercing the iterator must
// return a reference to the Object it was constructed with.
auto backwardIteratorTestDereference(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> sIt1(testSet);
    riot::SequenceIterator<int> sIt2(testSet + 1);
    riot::SequenceIterator<int> sIt3(testSet + 2);
    riot::BackwardIterator<riot::SequenceIterator<int> > it1(sIt1);
    riot::BackwardIterator<riot::SequenceIterator<int> > it2(sIt2);
    riot::BackwardIterator<riot::SequenceIterator<int> > it3(sIt3);
    if (*it1 != testSet[0]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*it1 != testSet[0])\n");
        failedTests += 1;
        return;
    }
    if (*it2 != testSet[1]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*it2 != testSet[1])\n");
        failedTests += 1;
        return;
    }
    if (*it3 != testSet[2]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*it3 != testSet[2])\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test -> Operator. Expected Behavoir: After derefercing the iterator must
// deregerence to the Object it was constructed with.
auto backwardIteratorTestDereferencePointer(size_t& succeededTests, size_t& failedTests) -> void
{
    TestBackwardIteratorObj obj;
    riot::SequenceIterator<TestBackwardIteratorObj> sIt(&obj);
    riot::BackwardIterator<riot::SequenceIterator<TestBackwardIteratorObj> > it(sIt);
    it->increase();
    if (obj.cnt != 1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (obj.cnt != 1)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test Prefix ++: Expected Behavior: After incrementing the Iterator it should
// Point to the previous element in the sequence
auto backwardIteratorTestPrefixIncrease(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> sIt(testSet + 2);
    riot::BackwardIterator<riot::SequenceIterator<int> > it(sIt);
    if (*(++it) != testSet[1]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(++it1) != testSet[1])\n");
        failedTests += 1;
        return;
    }
    if (*(++it) != testSet[0]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(++it1) != testSet[0])\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test Prefix --: Expected Behavior: After decrementing the Iterator it should
// Point to the next element in the sequence
auto backwardIteratorTestPrefixDecrease(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> sIt(testSet);
    riot::BackwardIterator<riot::SequenceIterator<int> > it(sIt);
    if (*(--it) != testSet[1]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(--it1) != testSet[1])\n");
        failedTests += 1;
        return;
    }
    if (*(--it) != testSet[2]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(--it1) != testSet[2])\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all backward Iterator Tests
auto runBackwardIteratorTests(size_t& succeededTests, size_t& failedTests) -> void
{
    backwardIteratorTestEqualityFunction(succeededTests, failedTests);
    backwardIteratorTestNotEqualityFunction(succeededTests, failedTests);
    backwardIteratorTestDereference(succeededTests, failedTests);
    backwardIteratorTestDereferencePointer(succeededTests, failedTests);
    backwardIteratorTestPrefixIncrease(succeededTests, failedTests);
    backwardIteratorTestPrefixDecrease(succeededTests, failedTests);
}

#endif // BACKWARDITERATOR_TESTS_HPP
