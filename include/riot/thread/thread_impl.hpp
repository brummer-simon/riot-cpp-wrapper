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
typedef enum {
    Minimum = THREAD_PRIORITY_MIN,
    Idle    = THREAD_PRIORITY_IDLE,
    Main    = THREAD_PRIORITY_MAIN
} Priority;
} // namespace priority

namespace stacksize
{
typedef enum {
    Default = THREAD_STACKSIZE_DEFAULT,
    Minimum = THREAD_STACKSIZE_MINIMUM,
    Idle    = THREAD_STACKSIZE_IDLE,
    Main    = THREAD_STACKSIZE_MAIN
} Stacksize;
} // namespace stacksize

namespace status
{
typedef enum {
    NotFound       = STATUS_NOT_FOUND,
    Stopped        = STATUS_STOPPED,
    Sleeping       = STATUS_SLEEPING,
    MutexBlocked   = STATUS_MUTEX_BLOCKED,
    ReceiveBlocked = STATUS_RECEIVE_BLOCKED,
    SendBlocked    = STATUS_SEND_BLOCKED,
    ReplyBlocked   = STATUS_REPLY_BLOCKED,
    FlagBlockedAny = STATUS_FLAG_BLOCKED_ANY,
    FlagBlockedAll = STATUS_FLAG_BLOCKED_ALL,
    MboxBlocked    = STATUS_MBOX_BLOCKED,
    OnRunQueue     = STATUS_RUNNING,
    Running        = STATUS_RUNNING,
    Pending        = STATUS_PENDING
} Status;
} // namespace status

// Forward declaration thread function wrapper.
namespace keepout
{
template <std::size_t Stacksize, typename Context>
auto func(void * thread) -> void *;
} // namespace keepout
} // namespace thread

template<std::size_t Stacksize = thread::stacksize::Default, typename Context>
class Thread
{
public:
    // Define signature of thread task.
    typedef auto (*Task) (Thread<Stacksize> & thread) -> void;

    Thread(Task const task, uint8_t const priority)
        : Thread(task, priority, "")
    {
    }

    Thread(Task const task, uint8_t const priority, char const * const name)
        : task_(task)
        , priority_(priority)
        , name_(name)
    {
        // Start thread
        thread_create(this->stack_, sizeof(this->stack_), this->priority_,
                      0, thread::keepout::func<Stacksize, Context>, this, this->name_);
    }

    auto getPid() const -> int
    {
        return this->pid_;
    }

    auto getName() const -> char const *
    {
        return this->name_;
    }

    auto getStatus() const -> thread::status::Status
    {
        return static_cast<thread::status::Status>(thread_getstatus(this->pid_));
    }

private:
    Task task_;                /**< Thread task */
    uint8_t priority_;         /**< Thread priority */
    char const * const name_;  /**< Thread name */
    char stack_[Stacksize];    /**< Thread stack */
    kernel_pid_t pid_;         /**< Thread Id of running thread */
    Context contex_;

    friend auto thread::keepout::func<Stacksize, Context>(void * thread) -> void *;
};

namespace thread
{
namespace keepout
{
template <std::size_t Stacksize, typename Context>
auto func(void * thread) -> void *
{
    // Get Reference to thread Object
    Thread<Stacksize>& ref = *((Thread<Stacksize, Context> *) thread);

    // Set Threads PID
    ref.pid_ = thread_getpid();

    // Execute Thread Function
    ref.task_(ref);

    return NULL;
}
} // namespace keepout
} // namespace thread
} // namespace riot

#endif // THREAD_IMPL_HPP
