/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef TESTLOCK_HPP
#define TESTLOCK_HPP

#include <cstdint>

class TestLock
{
public:
    TestLock()
        : timesLocked(0)
        , timesUnlocked(0)
    {
    }

    auto lock() -> void
    {
        this->timesLocked += 1;
    }

    auto unlock() -> void
    {
        this->timesUnlocked += 1;
    }

    uint32_t timesLocked;
    uint32_t timesUnlocked;
};

#endif // TESTOBJ
