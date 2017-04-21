/*
 * Copyright (C) 2017 Simon Brummer <simon.brummer@posteo.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     riot_cpp_wrapper
 * @{
 *
 * @file
 * @brief       Stack for RIOT-OS thread wrapper implementation.
 *
 * @author      Simon Brummer <simon.brummer@posteo.de>
 *
 * @}
 */

#ifndef THREADSTACK_IMPL_HPP
#define THREADSTACK_IMPL_HPP

#include <cstdint>
#include <thread.h>

namespace riot
{
namespace thread
{
namespace stacksize
{
/**
 * @brief Enum containing the different, predefined stacksizes.
 */
typedef enum {
    Default = THREAD_STACKSIZE_DEFAULT,
    Minimum = THREAD_STACKSIZE_MINIMUM,
    Idle    = THREAD_STACKSIZE_IDLE,
    Main    = THREAD_STACKSIZE_MAIN
} Stacksize;
} // namespace stacksize

/**
 * @brief Stack intended to use with riot::Thread.
 */
template<std::size_t Size = stacksize::Default, uint8_t GuardPattern = 0xA5>
class Stack
{
public:
    /**
     * @brief Default Constructor.
     */
    Stack()
        : topguard_(GuardPattern)
        , botguard_(GuardPattern)
    {
    }

    /**
     * @brief Get pointer to stacks base address.
     * @returns   Base address of stack.
     */
    auto getBase() -> uint8_t * const
    {
        return mem_;
    }

    /**
     * @brief Get stack size in bytes.
     * @returns   Stack size in bytes.
     */
    auto getSize() const -> std::size_t
    {
        return Size;
    }

    /**
     * @brief Check if Stack is in a valid state, meaning the stack is in its memory boundries.
     * @note A specific bit pattern is used to verify that the stack didn't overflow.
     *       There is a small chance that the value, the stack overflows with, is the same
     *       as the configured value, leading to non working verification.
     *       The bit pattern can be changed to a different value (second template parameter).
     * @returns   Zero if the stack is valid.
     *            Positive value if the stack overflowed.
     *            Negative value if the stack underflowed.
     */
    auto valid() const -> int
    {
        if (this->topguard_ != GuardPattern) {
            return 1;
        }
        if (this->botguard_ != GuardPattern) {
            return -1;
        }
        return 0;
    }

private:
    uint8_t const topguard_;    /**< Byte used to check if thread stack is within its boundries */
    uint8_t mem_[Size];         /**< Memory area used as thread Stack */
    uint8_t const botguard_;    /**< Byte used to check if thread stack is within its boundries */

    // Deleted on purpose
    Stack(Stack const & other) = delete;
    Stack(Stack const && other) = delete;
    auto operator = (Stack const & other) -> Stack & = delete;
    auto operator = (Stack const && other) -> Stack & = delete;
};

} // namespace thread
} // namespace riot
#endif // THREADSTACK_IMPL_HPP
