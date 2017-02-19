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
  * @brief       Dummy Lock, implements Lock interface without doing anything.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef DUMMY_LOCK_IMPL_HPP
#define DUMMY_LOCK_IMPL_HPP

namespace riot
{
namespace keep_out
{

/**
 * @brief dummy_lock complies lock interface without doing anything.
 */
class dummy_lock {
public:
    /**
     * @brief Dummy lock() method.
     */
    auto lock() -> void {
    }

    /**
     * @brief Dummy unlock() method.
     */
    auto unlock() -> void {
    }
};

} // namespace keep_out
} // namespace riot
#endif // DUMMY_LOCK_IMPL_HPP
