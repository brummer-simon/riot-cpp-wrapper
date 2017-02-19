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
  * @brief       C++ RAII compliant lock_guard.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef LOCK_GUARD_IMPL_HPP
#define LOCK_GUARD_IMPL_HPP

namespace riot
{

/**
 * @brief Implements RAII styled lock_guard.
 *        Usage: The supplied lock will be locked on construction and unlocked
 *        on destruction. The lock must be valid during the lifetime of the
 *        lock_guard.
 */
template<typename lock_type_t>
class lock_guard {
public:
    /**
     * @brief Constructor. Trys to aquire given lock.
     *
     * @param[in] lock   Lock that should be aquired.
     */
    lock_guard(lock_type_t& lock)
        : lock(lock)
    {
        lock.lock();
    }

    /**
     * @brief Destructor. Releases the previously aquired lock.
     */
    ~lock_guard() {
        lock.unlock();
    }

private:
    lock_type_t& lock; /**< Reference to the lock_guards lock */

    // Deleted with purpose
    lock_guard(const lock_guard& other) = delete;
    lock_guard(const lock_guard&& other) = delete;
    lock_guard& operator = (const lock_guard& other) = delete;
    lock_guard& operator = (const lock_guard&& other) = delete;
};

} // namespace riot
#endif // LOCK_GUARD_IMPL_HPP
