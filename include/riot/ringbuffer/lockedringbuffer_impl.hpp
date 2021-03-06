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
  * @brief       Threadsafe Ringbuffer. All ringbuffer operations are
  *              synchronized via an internal Lock.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef LOCKEDRINGBUFFER_IMPL_HPP
#define LOCKEDRINGBUFFER_IMPL_HPP

#include <initializer_list>
#include "../mutex/mutex_impl.hpp"
#include "../mutex/lockguard_impl.hpp"
#include "ringbuffer_impl.hpp"

namespace riot
{

// Forward declaration LockedRingbuffer
template <typename T, std::size_t Size, typename Buffer, typename Lock>
class LockedRingbuffer;

// Forward declaration swap function (needs friend access)
template <typename T, std::size_t Size, typename Buffer, typename Lock>
auto swap(LockedRingbuffer<T, Size, Buffer, Lock> & lhs,
          LockedRingbuffer<T, Size, Buffer, Lock> & rhs) -> void;

// Implementation LockedRingbuffer
template <typename T, std::size_t Size, typename Buffer = Ringbuffer<T, Size>,
          typename Lock = Mutex>
class LockedRingbuffer
{
public:
    // Use Membertypes of internal Ringbuffer
    typedef T ValueType;
    typedef T & Reference;
    typedef T * Pointer;
    typedef T const & ConstReference;
    typedef T const * ConstPointer;
    typedef std::size_t SizeType;

    /**
     * @brief Default Constructor: Creates empty LockedRingbuffer
     */
    LockedRingbuffer()
    {
    }

    /**
     * @brief Initializer list Constructor.
     * @see Documentation of supplied template T.
     */
    LockedRingbuffer(std::initializer_list<ValueType> const & list)
        : buffer_(Buffer(list))
    {
    }

    /**
     * @brief Fill - Constructor.
     * @see Documentation of supplied template T.
     */
    LockedRingbuffer(ConstReference initValue)
        : buffer_(initValue)
    {
    }

    /**
     * @brief Fill - Constructor.
     * @see Documentation of supplied template T.
     */
    LockedRingbuffer(ConstReference initValue, SizeType const n)
        : buffer_(initValue, n)
    {
    }

    /**
     * @brief Copy - Constructor.
     * @see Documentation of supplied template T.
     */
    LockedRingbuffer(LockedRingbuffer const & other)
    {
        riot::LockGuard<Lock> guardOther(other.lock_);
        this->buffer_ = other.buffer_;
    }

    /**
     * @brief Synchronized assignment operator.
     * @see Documentation operator =() of supplied template T.
     */
    auto operator = (LockedRingbuffer const & rhs) -> LockedRingbuffer &
    {
        if (this != &rhs) {
            // Aquire both locks
            riot::LockGuard<Lock> guardLhs(this->lock_);
            riot::LockGuard<Lock> guardRhs(rhs.lock_);
            this->buffer_ = rhs.buffer_;
        }
        return *this;
    }

    /**
     * @brief Synchronized addOne().
     * @see Documentation addOne() of supplied template T.
     */
    auto addOne(ConstReference src) -> void
    {
        riot::LockGuard<Lock> guard(this->lock_);
        this->buffer_.addOne(src);
    }

    /**
     * @brief Synchronized addOne().
     * @see Documentation addOne() of supplied template T.
     */
    auto addOne(ConstReference src, Reference removed) -> int
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.addOne(src, removed);
    }

    /**
     * @brief Synchronized putOne().
     * @see Documentation putOne() of supplied template T.
     */
    auto putOne(ConstReference src) -> int
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.putOne(src);
    }

    /**
     * @brief Synchronized getOne().
     * @see Documentation getOne() of supplied template T.
     */
    auto getOne(Reference dst) -> int
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.getOne(dst);
    }

    /**
     * @brief Synchronized peekOne().
     * @see Documentation peekOne() of supplied template T.
     */
    auto peekOne(Reference dst) const -> int
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.peekOne(dst);
    }

    /**
     * @brief Synchronized add().
     * @see Documentation add() of supplied template T.
     */
    auto add(ValueType src[], SizeType n) -> SizeType
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.add(src, n);
    }

    /**
     * @brief Synchronized get().
     * @see Documentation get() of supplied template T.
     */
    auto get(ValueType dst[], SizeType n) -> SizeType
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.get(dst, n);
    }

    /**
     * @brief Synchronized peek().
     * @see Documentation peek() of supplied template T.
     */
    auto peek(ValueType dst[], SizeType n) const -> SizeType
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.peek(dst, n);
    }

    /**
     * @brief Synchronized getFree().
     * @see Documentation getFree() of supplied template T.
     */
    auto getFree() const -> SizeType
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.getFree();
    }

    /**
     * @brief Synchronized empty().
     * @see Documentation empty() of supplied template T.
     */
    auto empty() const -> int
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.empty();
    }

    /**
     * @brief Synchronized full().
     * @see Documentation full() of supplied template T.
     */
    auto full() const -> int
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.full();
    }

    /**
     * @brief Synchronized remove().
     * @see Documentation remove() of supplied template T.
     */
    auto remove(SizeType const n) -> SizeType
    {
        riot::LockGuard<Lock> guard(this->lock_);
        return this->buffer_.remove(n);
    }

private:
    Buffer buffer_;
    mutable Lock lock_;

    // Friend Function needed to aquire internal locks ...
    friend auto swap<T, Size, Buffer, Lock>(LockedRingbuffer& lhs, LockedRingbuffer& rhs) -> void;
};

template <typename T, std::size_t Size, typename Buffer, typename Lock>
auto swap(LockedRingbuffer<T, Size, Buffer, Lock>& lhs,
          LockedRingbuffer<T, Size, Buffer, Lock>& rhs) -> void
{
    riot::LockGuard<Lock> guardLhs(lhs.lock_);
    riot::LockGuard<Lock> guardRhs(rhs.lock_);
    swap(lhs.buffer_, rhs.buffer_);
}

} // namespace riot
#endif // LOCKEDRINGBUFFER_IMPL_HPP
