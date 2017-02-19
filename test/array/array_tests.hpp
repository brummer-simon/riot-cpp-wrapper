/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef ARRAY_TESTS_HPP
#define ARRAY_TESTS_HPP

#include "riot/array.hpp"

// Test Constructors. Expected Behavior: The init constructor should initialize
// all elements with a given value. The initializer list constructor assigns
// every element of the init list to the array element at the same position.
auto arrayTestConstructor(size_t& succeededTests, size_t& failedTests) -> void
{
    // Init constructor
    riot::Array<int, 3> a(-23);
    if (a[0] != -23 || a[1] != -23 || a[2] != -23) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a[0] != -23 || a[1] != -23 || a[2] != -23)\n\n");
        failedTests += 1;
        return;
    }
    // initializer_list
    riot::Array<int, 3> b = {23, 42, -1};
    if (b[0] != 23 || b[1] != 42 || b[2] != -1) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (b[0] != 23 || b[1] != 42 || b[2] != -1)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test [] Operator. Expected Behavoir. Access should return a Reference
// to the element at the given position.
auto arrayTestAccess(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Array<int, 3> a = {23, 42, -1};
    if (a[0] != 23 || a[1] != 42 || a[2] != -1) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a[0] != 23 || a[1] != 42 || a[2] != -1)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test of 'at()' Method. Expected Behavoir: Provide access to an arrays element
// via an elements index. at() performs a boundry check in the supplied index.
// If the index is valid: Return == Ref to element at position idx the
// err flag is zero. If index is out of bounds, return value should be to
// the first array element and err is -EINVAL.
auto arrayTestAt(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Array<int, 3> testArray = {0, 1, 2};
    int err = 0;
    // Access existing element
    if (testArray.at(1, err) != 1) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (testArray.at(1, err) != 1)\n\n");
        failedTests += 1;
        return;
    }
    if (err != 0) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (err != 0)\n\n");
        failedTests += 1;
        return;
    }
    // Access non-existing element
    if (testArray.at(3, err) != 0) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (testArray.at(3, err) != 0)\n\n");
        failedTests += 1;
        return;
    }
    if (err != -EINVAL) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (err != -EINVAL)\n\n");
        failedTests += 1;
        return;
    }
    if (testArray.at(-1, err) != 0) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (testArray.at(3, err) != 0)\n\n");
        failedTests += 1;
        return;
    }
    if (err != -EINVAL) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (err != -EINVAL)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test data(). Expected Behavior: Return Pointer to internal Array. Accessing
// This Pointer must be the same as accessing the array.
auto arrayTestData(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Array<int, 3> a = {23, 42, -1};
    int *pa = a.data();
    if (a[0] != pa[0] || a[1] != pa[1] || a[2] != pa[2]) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a[0] != pa[0] || a[1] != pa[1] || a[2] != pa[2])\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test iterator. Expected Behavior: Forward Iterator. begin() must generate
// an iterator to the first element, ++ advances the iterator to the next
// element, -- to the previous element, end() return an iterator, past-the-end
// element. A ReverseIterator delivers the same operations with reversed logic.
auto arrayTestIterators(size_t& succeededTests, size_t& failedTests) -> void
{
    // Check Forward Iterator
    riot::Array<int, 2> a = {23, 42};
    riot::Array<int, 2>::Iterator it = a.begin();
    if (*it != 23 || *(++it) != 42 || ++it != a.end()) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (*it != 23 || *(++it) != 42 || ++it != b.end())\n\n");
        failedTests += 1;
        return;
    }
    it = a.end();
    if (*(--it) != 42 || *(--it) != 23 || it != a.begin()) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (*(--it) != 42 || *(--it) != 23 || it != a.begin())\n\n");
        failedTests += 1;
        return;
    }
    // Check BackwardIterator
    riot::Array<int, 2>::ReverseIterator rit = a.rbegin();
    if (*rit != 42 || *(++rit) != 23 || ++rit != a.rend()) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (*rit != 42 || *(++rit) != 23 || ++rit != a.rend())\n\n");
        failedTests += 1;
        return;
    }
    rit = a.rend();
    if (*(--rit) != 23 || *(--rit) != 42 || rit != a.rbegin()) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (*(--rit) != 23 || *(--rit) != 42 || rit != a.rbegin())\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test size(). Expected Behavior: size() must return the size specified in the
// second template parameter.
auto arrayTestSize(size_t& succeededTests, size_t& failedTests) -> void
{
    // Check Forward Iterator
    riot::Array<int, 3> a = {23, 42, -1};
    if (a.size() != 3) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a.size() != 3)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test fill(). Expected Behavior: fill() overrides every element in the
// array with the given value.
auto arrayTestFill(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Array<int, 3> a = {23, 42, -1};
    a.fill(0);
    if (a[0] != 0 || a[1] != 0 || a[2] != 0) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a[0] != 0 || a[1] != 0 || a[2] != 0)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test swap(). Expected Behavior: swaps the content of two arrays of the
// same Type and size.
auto arrayTestSwap(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Array<int, 3> a = {1, 2, 3};
    riot::Array<int, 3> b = {4, 5, 6};
    a.swap(b);
    if (a[0] != 4 || a[1] != 5 || a[2] != 6) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a[0] != 4 || a[1] != 5 || a[2] != 6)\n\n");
        failedTests += 1;
        return;
    }
    if (b[0] != 1 || b[1] != 2 || b[2] != 3) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (b[0] != 1 || b[1] != 2 || b[2] != 3)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test == operator. Expected Behavior: comparisson of two arrays must return
// true if the have equal content (equal element at equal position). the
// comparisson must return false if the Arrays contents differ.
auto arrayTestEqual(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Array<int, 3> a = {23, 42, -1};
    riot::Array<int, 3> b = {23, 42, -1};
    riot::Array<int, 3> c = {23, 42, 0};
    if (!(a == b)) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (!(a == b))\n\n");
        failedTests += 1;
        return;
    }
    if (a == c) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a == c)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Test != operator. Expected Behavior: neq comparisson of two arrays must return
// true if the contents differ. The comparisson must return false
// if the Arrays contents are equal.
auto arrayTestNonEqual(size_t& succeededTests, size_t& failedTests) -> void
{
    riot::Array<int, 3> a = {23, 42, -1};
    riot::Array<int, 3> b = {23, 42, 0};
    riot::Array<int, 3> c = {23, 42, -1};
    if (!(a != b)) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (!(a != b))\n\n");
        failedTests += 1;
        return;
    }
    if (a != c) {
        printf("Test '%s' failed.", __PRETTY_FUNCTION__);
        printf(" Reason: (a != c)\n\n");
        failedTests += 1;
        return;
    }
    printf("Test '%s' succeeded.\n\n", __PRETTY_FUNCTION__);
    succeededTests += 1;
}

// Run all Array Tests
auto runArrayTests(size_t& succeededTests, size_t& failedTests) -> void
{
    arrayTestConstructor(succeededTests, failedTests);
    arrayTestAccess(succeededTests, failedTests);
    arrayTestAt(succeededTests, failedTests);
    arrayTestData(succeededTests, failedTests);
    arrayTestIterators(succeededTests, failedTests);
    arrayTestSize(succeededTests, failedTests);
    arrayTestFill(succeededTests, failedTests);
    arrayTestSwap(succeededTests, failedTests);
    arrayTestEqual(succeededTests, failedTests);
    arrayTestNonEqual(succeededTests, failedTests);
}

#endif // ARRAY_TESTS_HPP
