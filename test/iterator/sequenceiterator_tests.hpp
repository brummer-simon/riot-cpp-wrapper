/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef SEQUENCEITERATOR_TESTS_HPP
#define SEQUENCEITERATOR_TESTS_HPP

#include "riot/iterator/sequenceiterator_impl.hpp"

struct TestSequenceIteratorObj
{
    int cnt;
    TestSequenceIteratorObj() : cnt(0) {}
    auto increase() -> void {++cnt;}
};

// Test == Function: Expected Behavior: Two SequenceIterators pointing to the
// same location must be equivalent -> it1 == it2 must be equal to it2 == it1.
auto sequenceIteratorTestEqualityFunction(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> it1(testSet);
    riot::SequenceIterator<int> it2(testSet);
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

// Test != Function: Expected Behavior: Two SequenceIterators pointing to the
// different locations must be not equival -> it1 != it2 must be equal to it2 != it1.
auto sequenceIteratorTestNotEqualityFunction(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> it1(testSet);
    riot::SequenceIterator<int> it2(testSet + 1);
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
auto sequenceIteratorTestDereference(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> it1(testSet);
    riot::SequenceIterator<int> it2(testSet + 1);
    riot::SequenceIterator<int> it3(testSet + 2);
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
auto sequenceIteratorTestDereferencePointer(size_t& succeededTests, size_t& failedTests) -> void
{
    TestSequenceIteratorObj obj;
    riot::SequenceIterator<TestSequenceIteratorObj> it(&obj);
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
// Point to the next element in the sequence
auto sequenceIteratorTestPrefixIncrease(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> it1(testSet);
    if (*(++it1) != testSet[1]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(++it1) != testSet[1])\n");
        failedTests += 1;
        return;
    }
    if (*(++it1) != testSet[2]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(++it1) != testSet[2])\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test Prefix --: Expected Behavior: After decrementing the Iterator it should
// Point to the previous element in the sequence
auto sequenceIteratorTestPrefixDecrease(size_t& succeededTests, size_t& failedTests) -> void
{
    int testSet[3] = {0, 1, 2};
    riot::SequenceIterator<int> it1(testSet + 2);
    if (*(--it1) != testSet[1]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(--it1) != testSet[1])\n");
        failedTests += 1;
        return;
    }
    if (*(--it1) != testSet[0]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (*(--it1) != testSet[0])\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all sequence Iterator Tests
auto runSequenceIteratorTests(size_t& succeededTests, size_t& failedTests) -> void
{
    sequenceIteratorTestEqualityFunction(succeededTests, failedTests);
    sequenceIteratorTestNotEqualityFunction(succeededTests, failedTests);
    sequenceIteratorTestDereference(succeededTests, failedTests);
    sequenceIteratorTestDereferencePointer(succeededTests, failedTests);
    sequenceIteratorTestPrefixIncrease(succeededTests, failedTests);
    sequenceIteratorTestPrefixDecrease(succeededTests, failedTests);
}

#endif // SEQUENCEITERATOR_TESTS_HPP
