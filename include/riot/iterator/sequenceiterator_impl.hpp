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
  * @brief       Implementation: Iterator over sequential
  *              allocated contents (Array or Vector).
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef SEQUENCEITERATOR_IMPL_HPP
#define SEQUENCEITERATOR_IMPL_HPP

namespace riot
{

// Forward declaration of SequenceIterator.
template <typename T>
class SequenceIterator;

// Forward declaration of SequenceIterators friend functions.
template <typename T>
auto operator == (const SequenceIterator<T>& lhs,
                  const SequenceIterator<T>& rhs) -> bool;

// Implementation of SequenceIterator
template <typename T>
class SequenceIterator
{
public:
    // Member Types
    typedef T ValueType;
    typedef T& Reference;
    typedef T* Pointer;

    /**
     * @brief Fully-specified Constructor.
     * @param[in] ptr   Pointer type to iterator over.
     */
    explicit SequenceIterator(Pointer ptr)
        : ptr_(ptr)
    {
    }

    /**
     * @brief Move iterator to next element.
     * @returns   Ref to iterator, pointing to next element.
     */
    auto operator ++ () -> SequenceIterator&
    {
        this->ptr_ += 1;
        return *this;
    }

    /**
     * @brief Move iterator to previous element.
     * @returns   Ref to iterator, pointing to previous element.
     */
    auto operator -- () -> SequenceIterator&
    {
        this->ptr_ -= 1;
        return *this;
    }

    /**
     * @brief Dereference iterator.
     * @returns   Ref to the object, the iterator is pointing to.
     */
    auto operator * () const -> Reference
    {
        return *(this->ptr_);
    }

    /**
     * @brief Dereferences iterator.
     * @returns   Pointer to dereferenced object, the iterator is pointing to.
     */
    auto operator -> () const -> Pointer
    {
        return this->ptr_;
    }

private:
    Pointer ptr_;   /**< Pointer to the current element. */

    friend auto operator == <T>(const SequenceIterator& lhs,
                                const SequenceIterator& rhs) -> bool;
};

/**
 * @brief equal comparrison operator on SequenceIterators.
 * @param[in] lhs   Leftside of the operator.
 * @param[in] rhs   Rightside of the operator.
 * @returns         true if the Iterators pointing to the same location.
 */
template <typename T>
auto operator == (const SequenceIterator<T>& lhs,
                  const SequenceIterator<T>& rhs) -> bool
{
    return (lhs.ptr_ == rhs.ptr_);
}

/**
 * @brief not equal comparrison operator on SequenceIterators.
 * @param[in] lhs   Leftside of the operator.
 * @param[in] rhs   Rightside of the operator.
 * @returns         true if the Iterators pointing to different locations.
 */
template <typename T>
auto operator != (const SequenceIterator<T>& lhs,
                  const SequenceIterator<T>& rhs) -> bool
{
    return !(lhs == rhs);
}

} // namespace riot
#endif // SEQUENCEITERATOR_IMPL_HPP
