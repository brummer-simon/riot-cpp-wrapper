/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef ITERATOR_TESTS_HPP
#define ITERATOR_TESTS_HPP

#include "iteratorinterface_tests.hpp"
#include "sequenceiterator_tests.hpp"
#include "backwarditerator_tests.hpp"

// Run all Tests on Iterators
auto runIteratorTests(size_t& succeededTests, size_t& failedTests) -> void
{
    runIteratorInterfaceTests(succeededTests, failedTests);
    runSequenceIteratorTests(succeededTests, failedTests);
    runBackwardIteratorTests(succeededTests, failedTests);
}

#endif // ITERATOR_TESTS_HPP
