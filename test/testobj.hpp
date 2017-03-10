/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef TESTOBJ_HPP
#define TESTOBJ_HPP

#include <cstdint>

class TestObj
{
public:

    TestObj()
        : u8(0), u16(0), u32(0)
    {
    }

    TestObj(uint8_t u8, uint16_t u16, uint32_t u32)
        : u8(u8), u16(u16), u32(u32)
    {
    }

    auto sum() const -> uint32_t
    {
        return this->u32 + this->u16 + this->u8;
    }

    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
};

auto operator == (const TestObj& lhs, const TestObj& rhs) -> bool
{
    if (lhs.u8 != rhs.u8) {
        return false;
    }
    if (lhs.u16 != rhs.u16) {
        return false;
    }
    if (lhs.u32 != rhs.u32) {
        return false;
    }
    return true;
}

auto operator != (const TestObj& lhs, const TestObj& rhs) -> bool
{
    return !(lhs == rhs);
}

#endif // TESTOBJ
