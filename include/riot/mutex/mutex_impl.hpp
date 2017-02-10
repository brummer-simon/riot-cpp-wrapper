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
class mutex {
public:
    /**
     * @brief Default Constructor.
     */
    mutex() {
        mtx = MUTEX_INIT;
    }

    /**
     * @brief Destructor.
     */
    ~mutex() {
    }

    /**
     * @brief Trys to aquired the lock.
     *
     * @return 0   if the mutex is currently locked.
     * @return 1   if the mutex was unlocked. The mutex is now
                   locked by the calling thread.
     */
    auto try_lock() -> int {
        return mutex_trylock(&(this->mtx));
    }

    /**
     * @brief Locks the Mutex.
     *
     * @note Blocks until the mutex is successfully aquired.
     */
    auto lock() -> void {
        mutex_lock(&(this->mtx));
    }

    /**
     * @brief Unlocks the Mutex.
     */
    auto unlock() -> void {
        mutex_unlock(&(this->mtx));
    }

    /**
     * @brief Unlocks the Mutex and suspends the calling thread afterwards.
     */
    auto unlock_and_sleep() -> void {
        mutex_unlock_and_sleep(&(this->mtx));
    }

private:
    mutex_t mtx; /**< RIOT-OS mutex_t structure to perform operations on */

    // Deleted with purpose
    mutex(const mutex& other) = delete;
    mutex(const mutex&& other) = delete;
    mutex& operator = (const mutex& other) = delete;
    mutex& operator = (const mutex&& other) = delete;
};

} // namespace riot
#endif // MUTEX_IMPL_HPP
