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
  * @brief       C++ Wrapper for RIOT-OS mutex_t implementation.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef MUTEX_IMPL_HPP
#define MUTEX_IMPL_HPP

#include "mutex.h"

namespace riot
{

/**
 * @brief Wrapper for RIOT-OS mutex_t implementation. The mutex must be still
 *        locked and unlocked manually, for an automated lock and
 *        unlock mechanism see lock_guard.
 */

class Mutex
{
public:
    /**
     * @brief Default Constructor.
     */
    Mutex()
    {
        this->mutex_ = MUTEX_INIT;
    }

    /**
     * @brief Trys to aquired the lock.
     * @returns   Zero if the mutex is currently locked.
     *            One if the mutex was unlocked. The mutex is now
     *            locked by the calling thread.
     */
    auto tryLock() -> int
    {
        return mutex_trylock(&(this->mutex_));
    }

    /**
     * @brief Locks the Mutex.
     * @note Blocks until the mutex is successfully aquired.
     */
    auto lock() -> void
    {
        mutex_lock(&(this->mutex_));
    }

    /**
     * @brief Unlocks the Mutex.
     */
    auto unlock() -> void
    {
        mutex_unlock(&(this->mutex_));
    }

    /**
     * @brief Unlocks the Mutex and suspends the calling thread afterwards.
     */
    auto unlockAndSleep() -> void
    {
        mutex_unlock_and_sleep(&(this->mutex_));
    }

private:
    mutex_t mutex_; /**< RIOT-OS mutex_t structure to perform operations on */

    // Deleted with purpose
    Mutex(const Mutex& other) = delete;
    Mutex(const Mutex&& other) = delete;
    Mutex& operator = (const Mutex& other) = delete;
    Mutex& operator = (const Mutex&& other) = delete;
};

} // namespace riot
#endif // MUTEX_IMPL_HPP
