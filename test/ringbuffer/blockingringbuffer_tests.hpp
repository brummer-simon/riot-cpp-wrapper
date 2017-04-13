/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef BLOCKINGRINGBUFFER_TESTS_HPP
#define BLOCKINGRINGBUFFER_TESTS_HPP

#include "../testobj.hpp"
#include "riot/ringbuffer.hpp"

// Test Constructor. Expected Behavoir: Default constructor.
auto blockingRingbufferTestDefaultConstructor(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> br;
    (void) failedTests;
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test Constructor. Expected Behavoir: Copy as much element from initializer_list
// the Ringbuffer can store.
auto blockingRingbufferTestInitListConstructor(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 2> rbuf = {TestObj(1,2,3), TestObj(4,5,6)};
    TestObj ret1;
    TestObj ret2;
    rbuf.get(ret1);
    rbuf.get(ret2);
    if (ret1 != TestObj(1,2,3) || ret2 != TestObj(4,5,6)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret1 != TestObj(1,2,3) || ret2 != TestObj(4,5,6))\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test fill Constructor. Expected behavoir: Fill ringbuffer with up to n elements.
auto blockingRingbufferTestFillConstructor(size_t & succeededTests, size_t & failedTests) -> void
{
    // Fill-Constructor
    riot::BlockingRingbuffer<TestObj, 2> rbuf1(TestObj(1,2,3), 1);
    TestObj ret1;
    TestObj ret2;
    rbuf1.get(ret1);
    int err = rbuf1.tryGet(ret2);
    if (err != -EAGAIN || ret1 != TestObj(1,2,3)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret1 != ret2)\n");
        failedTests += 1;
    }
    riot::BlockingRingbuffer<TestObj, 2> rbuf2(TestObj(1,2,3));
    rbuf2.get(ret1);
    rbuf2.get(ret2);
    if (ret1 != ret2) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret1 != ret2)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test add(). Expected Behavior: Add a given Element to the BlockingRingbuffer
// if there is space available (returns zero). In case the ringbuffer is full, this function
// blocks until there is space available. -ECANCELED is returned is the semphores are
// destroyed (happens only on destructor call). -EOVERFLOW is not tested. Should not compile
// due to BlockingRingbuffer size limitations anyway.
auto blockingRingbufferTestAdd(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> br;
    TestObj obj(1,2,3);
    // Add Object. Must return zero
    int ret = br.add(obj);
    if (ret != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0)\n");
        failedTests += 1;
    }
    // Destroy Ringbuffer. Must return -ECANCELED
    br.~BlockingRingbuffer();
    ret = br.add(obj);
    if (ret != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test tryAdd(). Expected Behavior: Add a given Element to the BlockingRingbuffer
// if there is space available (returns zero). In case the ringbuffer is full, this function
// returns -EAGAIN (the element was not added). -ECANCELED is returned is the semphores are
// destroyed (happens only on destructor call). -EOVERFLOW is not tested. Should not compile
// due to BlockingRingbuffer size limitations anyway.
auto blockingRingbufferTestTryAdd(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> br;
    TestObj obj(1,2,3);
    // Add Object. Must return zero
    int ret = br.tryAdd(obj);
    if (ret != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0)\n");
        failedTests += 1;
    }
    // Ringbuffer is full. Must return -EAGAIN
    ret = br.tryAdd(obj);
    if (ret != -EAGAIN) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -EAGAIN)\n");
        failedTests += 1;
    }
    // Destroy Ringbuffer. Must return -ECANCELED
    br.~BlockingRingbuffer();
    ret = br.tryAdd(obj);
    if (ret != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test addTimed(). Expected Behavior: Add a given Element to the BlockingRingbuffer
// if there is space available (returns zero). In case the ringbuffer is full, this function
// blocks for timeout duration or until there is space available.
// In case the timeout expired: -ETIMEDOUT is returned. -ECANCELED is returned is the semphores are
// destroyed (happens only on destructor call). -EOVERFLOW is not tested. Should not compile
// due to BlockingRingbuffer size limitations anyway.
auto blockingRingbufferTestAddTimed(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> br;
    size_t timeout = 1000;
    TestObj obj(1,2,3);
    // Add Object. Must return zero
    int ret = br.addTimed(obj, timeout);
    if (ret != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0)\n");
        failedTests += 1;
    }
    // Add Object. Buffer is full -> Must timeout -> return -ETIMEOUT
    ret = br.addTimed(obj, timeout);
    if (ret != -ETIMEDOUT) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -ETIMEDOUT)\n");
        failedTests += 1;
    }
    // Destroy Ringbuffer. Must return -ECANCELED
    br.~BlockingRingbuffer();
    ret = br.addTimed(obj, timeout);
    if (ret != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test get(). Expected Behavior: Get a Element from the Ringbuffer if it is not empty (return zero).
// In case the ringbuffer is empty, this function blocks until there are elements available.
// -ECANCELED is returned is the semphores are destroyed (happens only on destructor call).
// -EOVERFLOW is not tested. Should not compile due to BlockingRingbuffer size limitations anyway.
auto blockingRingbufferTestGet(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> br;
    TestObj obj(1,2,3);
    TestObj ret;
    br.add(obj);
    // Get Object. Must return zero
    int err = br.get(ret);
    if (err != 0 || ret != obj) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != 0 || ret != obj)\n");
        failedTests += 1;
    }
    // Destroy Ringbuffer. Must return -ECANCELED
    br.~BlockingRingbuffer();
    err = br.get(ret);
    if (err != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test tryGet(). Expected Behavior: Get a Element from the Ringbuffer if it is not empty (return zero).
// In case the ringbuffer is empty, this function returns with -EAGAIN.
// -ECANCELED is returned is the semphores are destroyed (happens only on destructor call).
// -EOVERFLOW is not tested. Should not compile due to BlockingRingbuffer size limitations anyway.
auto blockingRingbufferTestTryGet(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> br;
    TestObj obj(1,2,3);
    TestObj ret;
    br.add(obj);
    // Get Object. Must return zero
    int err = br.tryGet(ret);
    if (err != 0 || ret != obj) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != 0 || ret != obj)\n");
        failedTests += 1;
    }
    // Get Object. Buffer is full Must return -EAGAIN
    err = br.tryGet(ret);
    if (err != -EAGAIN) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != -EAGAIN)\n");
        failedTests += 1;
    }
    // Destroy Ringbuffer. Must return -ECANCELED
    br.~BlockingRingbuffer();
    err = br.tryGet(ret);
    if (err != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test getTimed(). Expected Behavior: Get a Element from the Ringbuffer if it is not empty (return zero).
// In case the ringbuffer is empty, this function blocks for a given timeout. In case the timeout expired
// -ETIMEDOUT is returned. -ECANCELED is returned is the semphores are destroyed (happens only on destructor call).
// -EOVERFLOW is not tested. Should not compile due to BlockingRingbuffer size limitations anyway.
auto blockingRingbufferTestGetTimed(size_t & succeededTests, size_t & failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> br;
    size_t timeout = 1000;
    TestObj obj(1,2,3);
    TestObj ret;
    br.add(obj);
    // Get Object. Must return zero
    int err = br.getTimed(ret, timeout);
    if (err != 0 || ret != obj) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != 0 || ret != obj)\n");
        failedTests += 1;
    }
    // Get Object. Buffer is full -> Must timeout -> return -ETIMEOUT
    err = br.getTimed(ret, timeout);
    if (err != -ETIMEDOUT) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != -ETIMEDOUT)\n");
        failedTests += 1;
    }
    // Destroy Ringbuffer. Must return -ECANCELED
    br.~BlockingRingbuffer();
    err = br.getTimed(ret, timeout);
    if (err != -ECANCELED) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (err != -ECANCELED)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test getFree(): Expected behavoir: Returns the amount of additional elements the
// BlockingRingbuffer can store.
auto blockingRingbufferTestGetFree(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 2> rbuf;
    if (rbuf.getFree() != 2) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.getFree() != 2)\n");
        failedTests += 1;
        return;
    }
    rbuf.add(TestObj(1,2,3));
    if (rbuf.getFree() != 1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.getFree() != 1)\n");
        failedTests += 1;
        return;
    }
    rbuf.add(TestObj(1,2,3));
    if (rbuf.getFree() != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.getFree() != 0)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test empty() Method. Must return true after constructor, false after adding
// of a single Element and true after getting a single Element.
auto blockingRingbufferTestEmpty(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> rbuf;
    TestObj obj(1,2,3);
    // Constructed Ringbuffer must is empty
    if (!rbuf.empty()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        failedTests += 1;
        printf("!--- Reason: (!rbuf.empty())\n");
        return;
    }
    // Add Object to Ringbuffer. Buffer is not empty
    rbuf.add(obj);
    if (rbuf.empty()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.empty())\n");
        failedTests += 1;
        return;
    }
    // Remove Object to Ringbuffer. Buffer is empty
    rbuf.get(obj);
    if (!rbuf.empty()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (!rbuf.empty())\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test full() Method. Must return false after constructor, true after adding
// of a single Element and false after getting a single Element.
auto blockingRingbufferTestFull(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::BlockingRingbuffer<TestObj, 1> rbuf;
    TestObj obj(1,2,3);
    // Constructed Ringbuffer is not full.
    if (rbuf.full()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.full())\n");
        failedTests += 1;
        return;
    }
    // Add Object to Ringbuffer. Buffer is full
    rbuf.add(obj);
    if (!rbuf.full()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (!rbuf.full())\n");
        failedTests += 1;
        return;
    }
    // Remove Object to Ringbuffer. Buffer is not full
    rbuf.get(obj);
    if (rbuf.full()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.full())\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

auto runBlockingRingbufferTests(size_t & succeededTests, size_t & failedTests) -> void
{
    blockingRingbufferTestDefaultConstructor(succeededTests, failedTests);
    blockingRingbufferTestInitListConstructor(succeededTests, failedTests);
    blockingRingbufferTestFillConstructor(succeededTests, failedTests);
    blockingRingbufferTestAdd(succeededTests, failedTests);
    blockingRingbufferTestTryAdd(succeededTests, failedTests);
    blockingRingbufferTestAddTimed(succeededTests, failedTests);
    blockingRingbufferTestGet(succeededTests, failedTests);
    blockingRingbufferTestTryGet(succeededTests, failedTests);
    blockingRingbufferTestGetTimed(succeededTests, failedTests);
    blockingRingbufferTestGetFree(succeededTests, failedTests);
    blockingRingbufferTestEmpty(succeededTests, failedTests);
    blockingRingbufferTestFull(succeededTests, failedTests);
}

#endif // BLOCKINGRINGBUFFER_TESTS_HPP
