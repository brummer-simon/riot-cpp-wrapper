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

#include <thread.h>
#include "stack_impl.hpp"

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

auto getPid() -> int
{
    return thread_getpid();
}

// Forward declaration thread function wrapper.
namespace keepout
{
template <typename Context, std::size_t Stacksize>
auto func(void * thread) -> void *;
} // namespace keepout
} // namespace thread

template<typename Context, std::size_t Stacksize = thread::stacksize::Default>
class Thread
{
public:
    typedef thread::Stack<Stacksize> StackType;

    // Define signature of thread task, the user has to supply.
    typedef auto (*Task) (Thread<Context, Stacksize> & thread) -> void;

    Thread(Task const task, StackType & stack, uint8_t const priority, Context const & context)
        : Thread(task, stack, priority, context, "")
    {
    }

    Thread(Task const task, StackType & stack, uint8_t const priority, Context const & context,
           char const * const name)
        : task_(task)
        , context_(context)
        , name_(name)
    {
        // Start thread
        thread_create(reinterpret_cast<char *>(stack.getBase()), stack.getSize(), priority, 0,
                      thread::keepout::func<Context, Stacksize>, this, this->name_);
    }

    auto getPid() const -> int
    {
        return this->pid_;
    }

    auto getName() const -> char const *
    {
        return this->name_;
    }

    auto getContext() -> Context &
    {
        return this->context_;
    }

    auto getStatus() const -> thread::status::Status
    {
        return static_cast<thread::status::Status>(thread_getstatus(this->pid_));
    }

    auto join() -> void
    {
        // TODO: If thread is not stopped: Increase thread priority and yield ...
    }

private:
    Task task_;                /**< Thread task function */
    Context context_;          /**< Thread context for argument exchange */
    char const * const name_;  /**< Thread name */
    kernel_pid_t pid_;         /**< Thread Id of running thread */

    friend auto thread::keepout::func<Context, Stacksize>(void * thread) -> void *;
};

namespace thread
{
namespace keepout
{
template <typename Context, std::size_t Stacksize>
auto func(void * thread) -> void *
{
    // Get Reference to thread Object
    Thread<Context, Stacksize>& th = *((Thread<Context, Stacksize> *) thread);

    // Set Threads PID
    th.pid_ = thread::getPid();

    // Execute Thread Function
    th.task_(th);

    return NULL;
}
} // namespace keepout
} // namespace thread
} // namespace riot

#endif // THREAD_IMPL_HPP
