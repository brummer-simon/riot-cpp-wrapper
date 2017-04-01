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
   * @brief       C++ Wrapper for RIOT-OS thread implementation.
   *
   * @author      Simon Brummer <simon.brummer@posteo.de>
   *
   * @}
   */

#ifndef THREAD_IMPL_HPP
#define THREAD_IMPL_HPP

#include "thread.h"

namespace riot
{

namespace thread
{
namespace priority
{
enum {
    Minimum = THREAD_PRIORITY_MIN,
    Idle    = THREAD_PRIORITY_IDLE,
    Main    = THREAD_PRIORITY_MAIN
};
} // priority

namespace stacksize
{
enum {
    Default = THREAD_STACKSIZE_DEFAULT,
    Minimum = THREAD_STACKSIZE_MINIMUM,
    Idle    = THREAD_STACKSIZE_IDLE,
    Main    = THREAD_STACKSIZE_MAIN
};
} // stacksize

typedef void (*Task)(void *);
} // namespace thread


template<std::size_t Stacksize = thread::stacksize::Default>
class Thread
{
public:

private:
    thread::Task task_;
    uint8_t stack_[Stacksize];
};

} // namespace riot

#endif // THREAD_IMPL_HPP
