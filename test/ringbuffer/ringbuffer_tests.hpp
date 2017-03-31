/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef RINGBUFFER_TESTS_HPP
#define RINGBUFFER_TESTS_HPP

#include "../testobj.hpp"
#include "riot/ringbuffer.hpp"

// Test Constructor. Expected behavoir: Create empty Ringbuffer.
auto ringbufferTestDefaultConstructor(size_t& succeededTests, size_t& failedTests) -> void
{
    // Default Constructor
    riot::Ringbuffer<TestObj, 1> rbuf;
    rbuf.addOne(TestObj(1,2,3));
    TestObj ret;
    rbuf.getOne(ret);
    if (ret != TestObj(1,2,3)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != TestObj(1,2,3))\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test Initializer list Constructor. Expected behavoir: store as much elements as possible in Ringbuffer.
auto ringbufferTestInitializerListConstructor(size_t& succeededTests, size_t& failedTests) -> void
{
    // Initializer list Constructor
    riot::Ringbuffer<TestObj, 2> rbuf = {TestObj(1,2,3), TestObj(4,5,6)};
    TestObj ret1;
    TestObj ret2;
    rbuf.getOne(ret1);
    rbuf.getOne(ret2);
    if (ret1 != TestObj(1,2,3) || ret2 != TestObj(4,5,6)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret1 != TestObj(1,2,3) || ret2 != TestObj(4,5,6))\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test fill Constructor. Expected behavoir: Fill ringbuffer with up to n elements.
auto ringbufferTestFillConstructor(size_t& succeededTests, size_t& failedTests) -> void
{
    // Fill-Constructor
    riot::Ringbuffer<TestObj, 2> rbuf1(TestObj(1,2,3), 1);
    TestObj ret1;
    TestObj ret2;
    rbuf1.getOne(ret1);
    int err = rbuf1.getOne(ret2);
    if (err != - 1 || ret1 != TestObj(1,2,3)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret1 != ret2)\n");
        failedTests += 1;
    }
    riot::Ringbuffer<TestObj, 2> rbuf2(TestObj(1,2,3));
    rbuf2.getOne(ret1);
    rbuf2.getOne(ret2);
    if (ret1 != ret2) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret1 != ret2)\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test Copy Constructor. Expected behavoir: Copy object
auto ringbufferTestCopyConstructor(size_t& succeededTests, size_t& failedTests) -> void
{
    // Copy-Constructor
    riot::Ringbuffer<TestObj, 1> rbuf1 (TestObj(1,2,3));
    riot::Ringbuffer<TestObj, 1> rbuf2(rbuf1);
    TestObj ret;
    rbuf2.getOne(ret);
    if (ret != TestObj(1,2,3)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != TestObj(1,2,3))\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test Assignment operator. Expected behavoir: assign object.
auto ringbufferTestAssignmentOperator(size_t& succeededTests, size_t& failedTests) -> void
{
    // Assignment Operator
    riot::Ringbuffer<TestObj, 1> rbuf1 (TestObj(1,2,3));
    riot::Ringbuffer<TestObj, 1> rbuf2;
    rbuf2 = rbuf1;
    TestObj ret;
    rbuf2.getOne(ret);
    if (ret != TestObj(1,2,3)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != TestObj(1,2,3))\n");
        failedTests += 1;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test addOne(). Expected Behavior: The method adds an Element to the
// Ringbuffer. The add Methods override existing elements in case there is
// no left space. addOne(const T&, T&) should return -1 if no element
// was replaced, zero if a element was replaced (removed contains that element).
// addOne(const T&) just adds the element without returning the replaced element.
auto ringbufferTestAddOne(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 1> rbuf;
    TestObj in1(1,2,3);
    TestObj replaced;
    // Add to empty buffer
    int ret = rbuf.addOne(in1, replaced);
    if (ret != -1 || replaced != TestObj()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -1 || replaced != TestObj())\n");
        failedTests += 1;
        return;
    }
    // Add to full buffer. Element must be replaced.
    TestObj in2(4,5,6);
    ret = rbuf.addOne(in2, replaced);
    if (ret != 0 || replaced != in1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0 || replaced != in1)\n");
        failedTests += 1;
        return;
    }
    // Add to full buffer. Replaced element is discarded.
    TestObj in3(7,8,9);
    rbuf.addOne(in3);
    TestObj out;
    rbuf.getOne(out);
    if (in3 != out) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (in3 != out)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test add(). Expected Behavior: The add() Method add up the specified
// amount Elements from the given array and returns the number of added
// elements.
auto ringbufferTestAdd(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 4> rbuf;
    TestObj in[3] = {TestObj(1,2,3), TestObj(4,5,6), TestObj(7,8,9)};
    TestObj out[3];
    size_t ret = 0;

    ret = rbuf.add(in, 1);
    rbuf.getOne(out[0]);
    if (ret != 1 || out[0] != in[0]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 1 || out[0] != in[0])\n");
        failedTests += 1;
        return;
    }
    ret = rbuf.add(in, 3);
    rbuf.getOne(out[0]);
    rbuf.getOne(out[1]);
    rbuf.getOne(out[2]);
    if (ret != 3 ||out[0] != in[0] || out[1] != in[1] || out[2] != in[2]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 3 ||out[0] != in[0] || out[1] != in[1] ||\
                out[2] != in[2])\n");
        failedTests += 1;
        return;
    }
    rbuf.add(in, 3);
    ret = rbuf.add(in, 3);
    if (ret != 1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 1)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test putOne(): Expected behavior: putOne() places and element in the
// Ringbuffer if there is enough place to store the element. If the ringbuffer
// is full -ENOMEM is returned ant the element is not added. If return is zero
// the element was added to the Ringbuffer.
auto ringbufferTestPutOne(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 1> rbuf;
    TestObj in1(1,2,3);
    TestObj in2(1,2,3);
    TestObj out;
    int ret = rbuf.putOne(in1);
    rbuf.getOne(out);
    if (ret != 0 || in1 != out) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0 || in1 != out)\n");
        failedTests += 1;
        return;
    }
    // Buffer is empty, fill it again.
    rbuf.putOne(in1);
    ret = rbuf.putOne(in2);
    rbuf.getOne(out);
    if (ret != -ENOMEM || in1 != out) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -NOMEM || in1 != out)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test getOne(). Expected behavior: In case the Ringbuffer is empty, getOne()
// must not change the input parameter and it must return -1. If the Ringbuffer
// is not empty, the input parameter must contain the first added Object and the
// return value must be zero.
auto ringbufferTestGetOne(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 1> rbuf;
    TestObj out(23, 42, 1);
    int ret = rbuf.getOne(out);

    if (ret != -1 || out != TestObj(23, 42, 1)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -1 || out != TestObj(23, 42, 1))\n");
        failedTests += 1;
        return;
    }
    TestObj in(1, 2, 3);
    rbuf.addOne(in);
    ret = rbuf.getOne(out);
    if (ret != 0 || out != in) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0 || out != in)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test get(). Expected Behavoir, takes to a specified amount from the
// ringbuffer and returns the number of actualy taken elements.
auto ringbufferTestGet(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 2> rbuf;
    TestObj in[3] = {TestObj(1,2,3), TestObj(4,5,6), TestObj(7,8,9)};
    TestObj out[3];
    size_t ret = 0;

    rbuf.add(in, 3);
    ret = rbuf.get(out, 3);
    if (ret != 2 || out[0] != in[0] || out[1] != in[1]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 2 || out[0] != in[0] || out[1] != in[1])\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test peekOne(). Expected behavoir, returns a reference to the oldest element
// in the Ringbuffer with zero as a return value. -1 is returned if the Ringbuffer
// is empty.
auto ringbufferTestPeekOne(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 2> rbuf;
    TestObj out;
    int ret = rbuf.peekOne(out);
    if (ret != -1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != -1)\n");
        failedTests += 1;
        return;
    }
    TestObj in(1,2,3);
    rbuf.addOne(in);
    ret = rbuf.peekOne(out);
    if (ret != 0 || out != in) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0 || out != in)\n");
        failedTests += 1;
        return;
    }
    TestObj in2(4,5,6);
    rbuf.addOne(in2);
    ret = rbuf.peekOne(out);
    if (ret != 0 || out != in) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != 0 || out != in)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test peek(). Expected behavoir, put up to the specified amount of elements into
// the supplied array without removing them from the buffer.
auto ringbufferTestPeek(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 2> rbuf;
    TestObj in[3] = {TestObj(1,2,3), TestObj(4,5,6), TestObj(7,8,9)};
    TestObj out[3];
    TestObj peeked[3];
    size_t retPeek = 0;
    size_t retGet = 0;

    rbuf.add(in, 3);
    retPeek = rbuf.peek(peeked, 3);
    retGet = rbuf.get(out, 3);
    if (retPeek != 2 || retPeek != retGet || peeked[0] != out[0] ||
        peeked[1] != out[1]) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (retPeek != 2 || retPeek != retGet ||\
                peeked[0] != out[0] || peeked[1] != out[1])\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test addOne() and getOne(). Expected behavoir, two elements are added to the
// Ringbuffer, after adding two get Operations are performed. The result of the
// first getOne() must be equal to the first added object. The same goes for
// the second get and add.
auto ringbufferTestAddGet(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 2> rbuf;
    TestObj in1(1,2,3);
    TestObj in2(4,5,6);
    TestObj out1;
    TestObj out2;
    rbuf.addOne(in1);
    rbuf.addOne(in2);
    rbuf.getOne(out1);
    rbuf.getOne(out2);

    if (in1 != out1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (in1 != out1)\n");
        failedTests += 1;
        return;
    }
    if (in2 != out2) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (in2 != out2)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test getFree(): Expected behavoir: Returns the amount of additional elements the
// Ringbuffer can store.
auto ringbufferTestGetFree(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 2> rbuf;
    if (rbuf.getFree() != 2) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.getFree() != 2)\n");
        failedTests += 1;
        return;
    }
    rbuf.putOne(TestObj(1,2,3));
    if (rbuf.getFree() != 1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.getFree() != 1)\n");
        failedTests += 1;
        return;
    }
    rbuf.putOne(TestObj(1,2,3));
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
auto ringbufferTestEmpty(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 1> rbuf;
    TestObj obj(1,2,3);
    // Constructed Ringbuffer must is empty
    if (!rbuf.empty()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        failedTests += 1;
        printf("!--- Reason: (!rbuf.empty())\n");
        return;
    }
    // Add Object to Ringbuffer. Buffer is not empty
    rbuf.addOne(obj);
    if (rbuf.empty()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.empty())\n");
        failedTests += 1;
        return;
    }
    // Remove Object to Ringbuffer. Buffer is empty
    rbuf.getOne(obj);
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
auto ringbufferTestFull(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 1> rbuf;
    TestObj obj(1,2,3);
    // Constructed Ringbuffer is not full.
    if (rbuf.full()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.full())\n");
        failedTests += 1;
        return;
    }
    // Add Object to Ringbuffer. Buffer is full
    rbuf.addOne(obj);
    if (!rbuf.full()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (!rbuf.full())\n");
        failedTests += 1;
        return;
    }
    // Remove Object to Ringbuffer. Buffer is not full
    rbuf.getOne(obj);
    if (rbuf.full()) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.full())\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test remove(): Expected Behavior: This Function call removes atmost a given
// number of elements. In case the Ringbuffer contains less elements
// that the given n, it should be emptied and return
// the amount of acutally removed elements.
auto ringbufferTestRemove(size_t& succeededTests, size_t& failedTests) -> void
{
    // Fill Ringbuffer
    riot::Ringbuffer<TestObj, 3> rbuf;
    rbuf.putOne(TestObj(1,2,3));
    rbuf.putOne(TestObj(4,5,6));
    rbuf.putOne(TestObj(7,8,9));

    // Remove more elements than in the buffer
    if (rbuf.remove(4) != 3) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.remove(4) != 3)\n");
        failedTests += 1;
        return;
    }
    if (rbuf.remove(1) != 0) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (rbuf.removed(1) != 0)\n");
        failedTests += 1;
        return;
    }
    // Remove elements and check whats left.
    rbuf.putOne(TestObj(1,2,3));
    rbuf.putOne(TestObj(4,5,6));
    rbuf.remove(1);
    TestObj ret;
    rbuf.getOne(ret);
    if (ret != TestObj(4,5,6)) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (ret != TestObj(4,5,6))\n");
        failedTests += 1;
        return;
    }

    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test swap(): Expected behavior: Swaps the contents of two Ringbuffers.
auto ringbufferTestSwap(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Ringbuffer<TestObj, 1> rbuf1;
    riot::Ringbuffer<TestObj, 1> rbuf2;
    TestObj in1(1,1,1);
    TestObj in2(2,2,2);
    TestObj out1;
    TestObj out2;
    rbuf1.addOne(in1);
    rbuf2.addOne(in2);
    swap(rbuf1, rbuf2);
    rbuf1.getOne(out1);
    rbuf2.getOne(out2);
    if (in1 != out2 || in2 != out1) {
        printf("Test '%s' failed.\n", __PRETTY_FUNCTION__);
        printf("!--- Reason: (in1 != out2 || in2 != out1)\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all Ringbuffer Tests
auto runRingbufferTests(size_t& succeededTests, size_t& failedTests) -> void
{
    ringbufferTestDefaultConstructor(succeededTests, failedTests);
    ringbufferTestInitializerListConstructor(succeededTests, failedTests);
    ringbufferTestFillConstructor(succeededTests, failedTests);
    ringbufferTestCopyConstructor(succeededTests, failedTests);
    ringbufferTestAssignmentOperator(succeededTests, failedTests);
    ringbufferTestAddOne(succeededTests, failedTests);
    ringbufferTestAdd(succeededTests, failedTests);
    ringbufferTestPutOne(succeededTests, failedTests);
    ringbufferTestGetOne(succeededTests, failedTests);
    ringbufferTestGet(succeededTests, failedTests);
    ringbufferTestPeekOne(succeededTests, failedTests);
    ringbufferTestPeek(succeededTests, failedTests);
    ringbufferTestAddGet(succeededTests, failedTests);
    ringbufferTestGetFree(succeededTests, failedTests);
    ringbufferTestEmpty(succeededTests, failedTests);
    ringbufferTestFull(succeededTests, failedTests);
    ringbufferTestRemove(succeededTests, failedTests);
    ringbufferTestSwap(succeededTests, failedTests);
}

#endif // RINGBUFFER_TESTS_HPP
