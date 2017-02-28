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

#ifndef LOCKGUARD_IMPL_HPP
#define LOCKGUARD_IMPL_HPP

namespace riot
{

/**
 * @brief Implements RAII styled LockGuard.
 *        Usage: The supplied lock will be locked on construction and unlocked
 *        on destruction. The lock must be valid during the lifetime of the
 *        LockGuard.
 */
template<typename LockType>
class LockGuard
{
public:
    /**
     * @brief Constructor. Trys to aquire given lock.
     * @param[in] lock   Lock that should be aquired.
     */
    LockGuard(LockType& lock)
        : lock_(lock)
    {
        lock_.lock();
    }

    /**
     * @brief Destructor. Releases the previously aquired lock.
     */
    ~LockGuard()
    {
        lock_.unlock();
    }

private:
    LockType& lock_; /**< Reference to the lock_guards lock */

    // Deleted with purpose
    LockGuard(const LockGuard& other) = delete;
    LockGuard(const LockGuard&& other) = delete;
    LockGuard& operator = (const LockGuard& other) = delete;
    LockGuard& operator = (const LockGuard&& other) = delete;
};

} // namespace riot
#endif // LOCKGUARD_IMPL_HPP
