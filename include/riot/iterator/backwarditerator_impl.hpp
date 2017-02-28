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
  * @brief       Implementation: BackwardIterator adapter for iterators.
  *              Swaps ++ and -- operator, therefore they must exist.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef BACKWARDITERATOR_IMPL_HPP
#define BACKWARDITERATOR_IMPL_HPP

namespace riot
{

// Forward declaration of BackwardIterator.
template<typename IteratorType>
class BackwardIterator;

// Forward declaration of BackwardIterators friend functions.
template <typename IteratorType>
auto operator == (const BackwardIterator<IteratorType>& lhs,
                  const BackwardIterator<IteratorType>& rhs) -> bool;

// Implementation of BackwardIterator.
template<typename IteratorType>
class BackwardIterator
{
public:
    // Member Types
    typedef IteratorType ValueType;
    typedef IteratorType& Reference;
    typedef const IteratorType& ConstReference;

    /**
     * @brief Fully specified Constructor.
     * @param[in] iterator   Iterator that should be reversed.
     */
    explicit BackwardIterator(ConstReference& iterator)
        : iterator_(iterator)
    {
    }

    /**
     * @brief Move iterator to next element.
     * @returns   Ref to iterator, pointing to next element.
     */
    auto operator ++ () -> BackwardIterator&
    {
        --(this->iterator_);
        return *this;
    }

    /**
     * @brief Move iterator to previous element.
     * @returns   Ref to iterator, pointing to previous element.
     */
    auto operator -- () -> BackwardIterator&
    {
        ++(this->iterator_);
        return *this;
    }

    /**
     * @brief Dereference iterator.
     * @returns   Ref to the object, the internal iterator is pointing to.
     */
    auto operator * () const -> typename IteratorType::Reference
    {
        return this->iterator_.operator*();
    }

    /**
     * @brief Dereferences iterator.
     * @returns   Pointer to Dereferenced object, the iterator is pointing to.
     */
    auto operator -> () const -> typename IteratorType::Pointer
    {
        return this->iterator_.operator->();
    }

private:
    ValueType iterator_;   /**< Forward Iterator to reverse */

    friend auto operator == <IteratorType>(const BackwardIterator& lhs,
                                           const BackwardIterator& rhs) -> bool;
};

/**
 * @brief equal comparrison operator on Reverseiterators.
 * @param[in] lhs   Leftside of the operator.
 * @param[in] rhs   Rightside of the operator.
 * @returns         true if the memberiterators are equal.
 */
template <typename IteratorType>
auto operator == (const BackwardIterator<IteratorType>& lhs,
                  const BackwardIterator<IteratorType>& rhs) -> bool
{
    return (lhs.iterator_ == rhs.iterator_);
}

/**
 * @brief not equal comparrison operator on Reverseiterators.
 * @param[in] lhs   Leftside of the operator.
 * @param[in] rhs   Rightside of the operator.
 * @returns         true if the reverseiterators are not equal.
 */
template <typename IteratorType>
auto operator != (const BackwardIterator<IteratorType>& lhs,
                  const BackwardIterator<IteratorType>& rhs) -> bool
{
    return !(lhs == rhs);
}

} //namespace riot
#endif // BACKWARDITERATOR_IMPL_HPP
