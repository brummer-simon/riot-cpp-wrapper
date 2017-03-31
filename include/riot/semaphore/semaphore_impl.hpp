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
  * @brief       C++ Wrapper for RIOT-OS sema_t implementation.
  *              Requires 'sema' Module.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef SEMAPHORE_IMPL_HPP
#define SEMAPHORE_IMPL_HPP

#include <cstdint>
#include "sema.h"

namespace riot
{

/**
 * @brief Semaphore wrapper class.
 */
class Semaphore
{
public:
    /**
     * @brief Constructor
     * @param[in] value   The value the semaphore is initialized with.
     */
    Semaphore(std::size_t const value)
    {
        sema_create(&(this->sema_), value);
    }

    /**
     * @brief Destructor.
     * @note: Causes threads waiting on wait() and waitTime() to
     *        return with -ECANCELED.
     */
    ~Semaphore()
    {
        sema_destroy(&(this->sema_));
    }

    /**
     * @brief post operation on semaphore. Increases semaphore value.
     * @returns   Zero on succees.
     *            -EOVERFLOW, if semaphore value would overflow.
     */
    auto post() -> int
    {
        return sema_post(&(this->sema_));
    }

    /**
     * @brief wait operation on semaphore.
     * @note Blocks if semaphore value is less or equal zero.
     *       Until a post() call was performed.
     * @returns   Zero on success.
     *            -ECANCELED, if the semaphore was destroyed.
     */
    auto wait() -> int
    {
        return sema_wait(&(this->sema_));
    }

    /**
     * @brief Non-blocking wait() operation.
     * @returns   Zero on success.
     *            -EAGAIN, if the semaphore is not posted.
     *            -ECANCELED, if the semaphore was destroyed.
     */
    auto tryWait() -> int
    {
        return sema_try_wait(&(this->sema_));
    }

    /**
     * @brief wait operation with timeout.
     * @note Blocks if semaphore value is less or equal zero.
     *       Until a post() call was performed or a the timeout expired.
     * @param[in] timeout   Timout duration in microseconds.
     * @returns   Zero on success.
     *            -ETIMEDOUT, if the semaphore times out.
     *            -ECANCELED, if the semaphore was destroyed.
     */
    auto waitTimed(uint64_t const timeout) -> int
    {
        return sema_wait_timed(&(this->sema_), timeout);
    }

private:
    sema_t sema_; /**< Semaphore struct */

    // Deleted with purpose
    Semaphore(Semaphore const &) = delete;
    Semaphore(Semaphore const &&) = delete;
    auto operator = (Semaphore const &) -> Semaphore & = delete;
    auto operator = (Semaphore const &&) -> Semaphore & = delete;
};

} // namespace riot
#endif // SEMAPHORE_IMPL_HPP
