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
  * @brief       Wrapper class for RIOTs Ringbuffer implementation wrapper.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef RINGBUFFER_IMPL_HPP
#define RINGBUFFER_IMPL_HPP

#include <cstdint>
#include <cerrno>
#include "ringbuffer.h"

namespace riot
{

/**
 * @brief C++ wrapper for Ringbuffers.
 * @note The wrapped Ringbuffer operates on raw memory.
 *       This wrapper tries to avoid the pitfalls that come with
 *       memory operations on c++ objects.
 */
template <typename T, std::size_t size>
class Ringbuffer
{
public:
    // Member Types
    typedef T ValueType;
    typedef T& Reference;
    typedef T* Pointer;
    typedef const T& ConstReference;
    typedef const T* ConstPointer;
    typedef std::size_t SizeType;

    /**
     * @brief Default Constructor, creates empty Ringbuffer.
     */
    Ringbuffer()
    {
        ringbuffer_init(&(this->rbuf_), reinterpret_cast<char *>(this->mem_),
                        sizeof(ValueType) * size);
    }

    /**
     * @brief Copy Constructor.
     * @pre @p other must be copy-assignable.
     * @param[in] other   The Ringbuffer to copy.
     */
    Ringbuffer(const Ringbuffer& other)
    {
        for (SizeType i = 0; i < size; ++i) {
            this->mem_[i] = other.mem_[i];
        }
        this->rbuf_ = other.rbuf_;
        this->rbuf_.buf = reinterpret_cast<char *>(this->mem_);
    }

    /**
     * @brief Copy assignment operator.
     * @param[in] other   Object to assign to this object.
     */
    auto operator = (const Ringbuffer& other) -> Ringbuffer&
    {
        if (this != &other) {
            for (SizeType i = 0; i < size; ++i) {
                this->mem_[i] = other.mem_[i];
            }
            this->rbuf_ = other.rbuf_;
            this->rbuf_.buf = reinterpret_cast<char *>(this->mem_);
        }
        return *this;
    }

    /**
     * @brief Add an element to the Ringbuffer.
     * @note If ringbuffer is full: The oldest element gets overritten.
     * @param[in] src   Reference to object to place into ringbuffer.
     */
    auto addOne(ConstReference src) -> void
    {
        ValueType tmp;
        this->addOne(src, tmp);
    }

    /**
     * @brief Add an element to the Ringbuffer.
     * @note If ringbuffer is full: The oldest element is returned via @removed.
     * @param[in] src        Reference to object to place into ringbuffer.
     * @param[out] removed   Contains Object that was removed, if an Object
     *                       was removed.
     * @returns              zero if an element was replaced. @p removed
     *                       contains the removed element.
     *                       -1 if the buffer was not full. @p removed
     *                       was not changed.
     */
    auto addOne(ConstReference src, Reference removed) -> int
    {
        int ret = -1;
        if (this->full()) {
            this->getHead_(removed);
            ret = 0;
        }
        this->addTail_(src);
        return ret;
    }

    /**
     * @brief Put an element to Ringbuffer, if there is enough space left.
     * @param[in] src   Reference to object to place into ringbuffer.
     * @returns         Zero if element added to Ringbuffer.
     *                  -ENOMEM if Ringbuffer is full.
     */
    auto putOne(ConstReference src) -> int
    {
        if (this->full()) {
            return -ENOMEM;
        }
        this->addTail_(src);
        return 0;
    }

    /**
     * @brief Get oldest element from Ringbuffer.
     * @param[out] dst   Reference where the oldest element in the ringbuffer
     *                   Should be stored.
     * @return           Zero if a element was assigned to @p dst.
     *                   -1 if the Ringbuffer is empty.
     */
    auto getOne(Reference dst) -> int
    {
        if (this->empty()) {
            return -1;
        }
        this->getHead_(dst);
        return 0;
    }

    /**
     * @brief Get oldest element without removing it.
     * @param[out] dst   Reference to the oldest element in Ringbuffer.
     * @return           Zero if a element was assigned to @p dst.
     *                   -1 if the Ringbuffer is empty.
     */
    auto peekOne(Reference dst) -> int
    {
        if (this->empty()) {
            return -1;
        }
        dst = *(reinterpret_cast<ConstPointer>(this->rbuf_.buf +
                                               this->rbuf_.start));
        return 0;
    }

    /**
     * @brief Add up to @p n Elements to Ringbuffer.
     * @note This Method does not override any existing elements.
     * @param[in] src   Array of Elements to put into ringbuffer.
     * @param[in] n     Maximum Number of elements to take from @p src.
     * @returns         Number of added elements.
     */
    auto add(ValueType src[], SizeType n) -> SizeType
    {
        SizeType free = this->getFree();
        if (n > free) {
            n = free;
        }
        for (SizeType i = 0; i < n; ++i) {
            this->putOne(src[i]);
        }
        return n;
    }

    /**
     * @brief Take up to @p n elements from the Ringbuffer.
     * @param[out] dst   Array to store elements from the ringbuffer.
     * @param[in] n      Maximum number of elements to store in @p dst.
     * @returns          Number of actualy taken elements.
     */
    auto get(ValueType dst[], SizeType n) -> SizeType
    {
        SizeType avail = size - this->getFree();
        if (n > avail) {
            n = avail;
        }
        for (SizeType i = 0; i < n; ++i) {
            this->getOne(dst[i]);
        }
        return n;
    }

    /**
     * @brief Take up to @p n elements from the Ringbuffer without removing
     *        those elements.
     * @param[out] dst   Array to store elements from the ringbuffer.
     * @param[in] n      Maximum number of elements to store in @p dst.
     * @returns          Number of actualy peeked elements.
     */
    auto peek(ValueType dst[], SizeType n) -> SizeType
    {
        ringbuffer_t cpy = this->rbuf_;
        SizeType ret = this->get(dst, n);
        this->rbuf_ = cpy;
        return ret;
    }

    /**
     * @brief Number of elements that fit currently into ringbuffer.
     * @returns   Free places in Ringbuffer.
     */
    auto getFree() -> SizeType
    {
        return ringbuffer_get_free(&(this->rbuf_)) / sizeof(ValueType);
    }

    /**
     * @brief Check if Ringbuffer is empty.
     * @returns   non-zero if Ringbuffer is empty.
     *            zero if Ringbuffer contains elements.
     */
    auto empty() const -> int
    {
        return ringbuffer_empty(&(this->rbuf_));
    }

    /**
     * @brief Check if Ringbuffer is full.
     * @returns   non-zero if Ringbuffer is full.
     *            zero if Ringbuffer is not full.
     */
    auto full() const -> int
    {
        return ringbuffer_full(&(this->rbuf_));
    }

    /**
     * @brief Remove a number of elements from the Ringbuffer.
     * @param[in] n   At most number of elements to remove.
     * @returns       Actual number of removed elements.
     */
    auto remove(const SizeType n) -> SizeType
    {
        SizeType actualSize = n * sizeof(ValueType);
        SizeType ret = ringbuffer_remove(&(this->rbuf_), actualSize);
        if (ret == actualSize) {
            return n;
        }
        return (ret / sizeof(ValueType));
    }

    /**
     * @brief Swaps content with another Ringbuffer of same type and size.
     * @param[in,out] other   Ringbuffer to swap elements with.
     */
    auto swap(Ringbuffer& other) -> void
    {
        Ringbuffer tmp(other);
        other = *this;
        *this = tmp;
    }

private:
    /**
     * @brief Get oldest element from the ringbuffer.
     * @note Internal function, performs no boundry checks.
     * @pre ValueType must be copy-assignable.
     * @param[out] dst   Reference to assign oldest element in ringbuffer to.
     */
    auto getHead_(Reference dst) -> void
    {
        // Assign Object directly from current read position.
        // Hacky but avoids problems that come with memcpy.
        ConstPointer p = reinterpret_cast<ConstPointer>(this->rbuf_.buf +
                                                        this->rbuf_.start);
        dst = *p;

        // Read sizeof(ValueType) bytes from ringbuffer -> Removes Object.
        for (SizeType i = 0; i < sizeof(ValueType); ++i) {
            ringbuffer_get_one(&(this->rbuf_));
        }
    }

    /**
     * @brief Put element in the ringbuffer.
     * @note Internal function, performs no boundry checks.
     * @pre ValueType must be copy-assignable.
     * @param[in] src   Reference to object to put into ringbuffer.
     */
    auto addTail_(ConstReference src) -> void
    {
        // Calculate pointer to position to assign src to.
        SizeType offset = rbuf_.start + rbuf_.avail;
        if (offset >= rbuf_.size) {
            offset -= rbuf_.size;
        }
        Pointer p = reinterpret_cast<Pointer>(this->rbuf_.buf + offset);
        // Clear memory in Ringbuffer. Function updates Ringbuffer.
        for (SizeType i = 0; i < sizeof(ValueType); ++i) {
            ringbuffer_add_one(&(this->rbuf_), 0);
        }
        // Assign Object to pre-calculated position in Ringbuffer.
        *p = src;
    }

    ValueType mem_[size];   /**< Memory used for the ringbuffer */
    ringbuffer_t rbuf_;     /**< Wrapped ringbuffer struct */
};

} // namespace riot
#endif // RINGBUFFER_IMPL_HPP
