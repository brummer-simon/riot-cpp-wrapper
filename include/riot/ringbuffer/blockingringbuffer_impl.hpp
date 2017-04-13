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
  * @brief       Threadsafe ringbuffer with blocking queue semantics.
  *              Requires 'sema' Module.
  *
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef BLOCKINGRINGBUFFER_IMPL_HPP
#define BLOCKINGRINGBUFFER_IMPL_HPP

#include <initializer_list>
#include "../mutex.hpp"
#include "../semaphore/semaphore_impl.hpp"
#include "ringbuffer_impl.hpp"

namespace riot
{

template <typename T, std::size_t Size, typename Buffer = Ringbuffer<T, Size>,
          typename Lock = Mutex, typename Sema = Semaphore>
class BlockingRingbuffer
{
public:
    // Define Member types
    typedef T ValueType;
    typedef T & Reference;
    typedef T const & ConstReference;
    typedef std::size_t SizeType;

    /**
     * @brief Default Constructor. Create empty ringbuffer.
     */
    BlockingRingbuffer()
        : readerSema_(0)
        , writerSema_(Size)
    {
    }

    /**
     * @brief Constructor: Initialize ringbuffer with initializer_list. Copies
     *        up to 'Size' elements from @p li into constructed ringbuffer.
     * @param[in] list   Reference to initializer list.
     */
    BlockingRingbuffer(std::initializer_list<ValueType> const & list)
        : BlockingRingbuffer()
    {
        SizeType n = (Size < list.size()) ? Size : list.size();
        for (SizeType i = 0; i < n; ++i) {
            this->add(*(list.begin() + i));
        }
    }

    /**
     * @brief Fill-Constructor: Fill ringbuffer with up to @p n elements.
     * @param[in] initValue   Refernce to object the ringbuffer
     *                        should be filled with.
     * @param[in] n           Maximum Number of Elements of @p initValue, that
     *                        should be stored in ringbuffer.
     */
    BlockingRingbuffer(ConstReference initValue, SizeType const n)
        : BlockingRingbuffer()
    {
        SizeType size = (n < Size) ? n : Size;
        for (SizeType i = 0; i < size; ++i) {
            this->add(initValue);
        }
    }

    /**
     * @brief Fill-Constructor: Fill ringbuffer with given Element.
     * @param[in] initValue   Reference to object the ringbuffer
     *                        should be filled with.
     */
    BlockingRingbuffer(ConstReference initValue)
        : BlockingRingbuffer(initValue, Size)
    {
    }

    /**
     * @brief Add element to blocking ringbuffer.
     * @note Blocks if ringbuffer is full until an element has been removed
     *       with a get operation.
     * @param[in] src   Reference to object to place into ringbuffer.
     * @returns   Zero on success.
     *            -EOVERFLOW if reader semaphore overflowed.
     *            -ECANCELED if calling thread was blocked while blocking
     *            ringbuffer is destroyed.
     */
    auto add(ConstReference src) -> int
    {
        // Aquire writer semaphore
        int err = this->writerSema_.wait();
        if (err) {
            // Semaphore was destroyed.
            return err;
        }

        // Add Element. Semaphore usage ensures that putOne can't fail.
        this->lock_.lock();
        this->buffer_.putOne(src);
        this->lock_.unlock();

        // Post reader semaphore. Now there are elements in ringbuffer.
        err = this->readerSema_.post();
        if (err) {
            // Semaphore overflowed.
            return err;
        }
        return 0;
    }

    /**
     * @brief Get oldest element from ringbuffer.
     * @note Blocks if ringbuffer is empty until an element is added.
     * @param[out] dst   Reference to object there the aquired element
     *                   is stored into.
     * @returns   Zero on success.
     *            -EOVERFLOW if writer semaphore overflowed.
     *            -ECANCELED if calling thread was blocked while blocking
     *            ringbuffer is destroyed.
     */
    auto get(Reference dst) -> int
    {
        // Aquire reader semaphore
        int err = this->readerSema_.wait();
        if (err) {
            // Semaphore was destroyed.
            return err;
        }

        // Get Element from ringbuffer
        this->lock_.lock();
        this->buffer_.getOne(dst);
        this->lock_.unlock();

        // Post writer semaphore. Now there is space in ringbuffer
        err = this->writerSema_.post();
        if (err) {
            // Semaphore overflowed.
            return err;
        }
        return 0;
    }

    /**
     * @brief Try to add element to ringbuffer (non-blocking).
     * @param[in] src   Reference to object to place into ringbuffer.
     * @returns   Zero on success.
     *            -EAGAIN if ringbuffer is full. Element was not added to buffer.
     *            -EOVERFLOW if reader semaphore overflowed.
     *            -ECANCELED if ringbuffer is destroyed.
     */
    auto tryAdd(ConstReference src) -> int
    {
        // Try to aquire writer semaphore.
        int err = this->writerSema_.tryWait();
        if (err) {
            // Semaphore could not be aquired or semaphore was destroyed.
            return err;
        }

        // Add Element. Semaphore usage ensures that putOne can't fail.
        this->lock_.lock();
        this->buffer_.putOne(src);
        this->lock_.unlock();

        // Post reader semaphore. Now there are elements in ringbuffer
        err = this->readerSema_.post();
        if (err) {
            // Semaphore overflowed.
            return err;
        }
        return 0;
    }

    /**
     * @brief Try to get oldest element from ringbuffer (non-blocking).
     * @param[out] dst   Reference to object there the aquired element
     *                   is stored into.
     * @returns   Zero on success.
     *            -EAGAIN if ringbuffer is empty.
     *            -EOVERFLOW if writer semaphore overflowed.
     *            -ECANCELED if ringbuffer is destroyed.
     */
    auto tryGet(Reference dst) -> int
    {
        // Try to aquire reader semaphore
        int err = this->readerSema_.tryWait();
        if (err) {
            // Semaphore could not be aquired or semaphore was destroyed.
            return err;
        }

        // Get Element from ringbuffer
        this->lock_.lock();
        this->buffer_.getOne(dst);
        this->lock_.unlock();

        // Post writer semaphore. Now there is space in ringbuffer
        err = this->writerSema_.post();
        if (err) {
            // Semaphore overflowed.
            return err;
        }
        return 0;
    }

    /**
     * @brief Add element to ringbuffer. Blocks if buffer until the buffer can store the element
     *        or a timeout expired.
     * @param[in] src       Reference to object to place into ringbuffer.
     * @param[in] timeout   Timeout duration in microseconds.
     * @returns   Zero on success.
     *            -ETIMEDOUT if ringbuffer timeout expired after @timeout.
     *            -EOVERFLOW if reader semaphore overflowed.
     *            -ECANCELED if ringbuffer is destroyed.
     */
    auto addTimed(ConstReference src, uint64_t const timeout) -> int
    {
        // Try to aquire writer semaphore. Blocks if buffer is full
        int err = this->writerSema_.waitTimed(timeout);
        if (err) {
            // Wait operation timed out or semaphore was destroyed.
            return err;
        }

        // Add Element. Semaphore usage ensures that putOne can't fail.
        this->lock_.lock();
        this->buffer_.putOne(src);
        this->lock_.unlock();

        // Post reader semaphore. Now there are elements in ringbuffer
        err = this->readerSema_.post();
        if (err) {
            // Semaphore overflowed.
            return err;
        }
        return 0;
    }


    /**
     * @brief Get oldest element from ringbuffer. Blocks until the ringbuffer contains an
     *        element to get or a timeout expired.
     * @param[out] dst       Reference to object there the aquired element is stored into.
     * @param[in]  timeout   Timeout duration in microseconds.
     * @returns   Zero on success.
     *            -ETIMEDOUT if ringbuffer timeout expired after @timeout.
     *            -EOVERFLOW if writer semaphore overflowed.
     *            -ECANCELED if ringbuffer is destroyed.
     */
    auto getTimed(Reference dst, uint64_t const timeout) -> int
    {
        // Aquire reader semaphore. Blocks if buffer is empty
        int err = this->readerSema_.waitTimed(timeout);
        if (err) {
            // Wait operation timed out or semaphore was destroyed.
            return err;
        }

        // Get Element from ringbuffer
        this->lock_.lock();
        this->buffer_.getOne(dst);
        this->lock_.unlock();

        // Post writer semaphore. Now there is space in ringbuffer
        err = this->writerSema_.post();
        if (err) {
            // Semaphore overflowed.
            return err;
        }
        return 0;
    }

    /**
     * @brief Number of elements that fit currently into BlockingRingbuffer.
     * @returns   Free places in BlockingRingbuffer.
     */
    auto getFree() const -> SizeType
    {
        LockGuard<Lock> g(this->lock_);
        return this->buffer_.getFree();
    }

    /**
     * @brief Check if BlockingRingbuffer is empty.
     * @returns   non-zero if BlockingRingbuffer is empty.
     *            zero if BlockingRingbuffer contains elements.
     */
    auto empty() const -> int
    {
        LockGuard<Lock> g(this->lock_);
        return this->buffer_.empty();
    }

    /**
     * @brief Check if BlockingRingbuffer is full.
     * @returns   non-zero if BlockingRingbuffer is full.
     *            zero if BlockingRingbuffer is not full.
     */
    auto full() const -> int
    {
        LockGuard<Lock> g(this->lock_);
        return this->buffer_.full();
    }

private:
    Buffer buffer_;     /**< Ringbuffer implementation */
    mutable Lock lock_; /**< Mutex to synchronize access to buffer_ */
    Sema readerSema_;   /**< Reader Semaphore */
    Sema writerSema_;   /**< Writer Semaphore */

    // Deleted on purpose
    BlockingRingbuffer(BlockingRingbuffer const &) = delete;
    BlockingRingbuffer(BlockingRingbuffer const &&) = delete;
    auto operator = (BlockingRingbuffer const &) -> BlockingRingbuffer & = delete;
    auto operator = (BlockingRingbuffer const &&) -> BlockingRingbuffer & = delete;
};

} // namespace riot
#endif // BLOCKINGRINGBUFFER_IMPL_HPP
