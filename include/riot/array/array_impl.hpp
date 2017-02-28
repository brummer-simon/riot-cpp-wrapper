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
  * @brief       C-Array wrapper.
  *
  * @author      Simon Brummer <simon.brummer@posteo.de>
  *
  * @}
  */

#ifndef ARRAY_IMPL_HPP
#define ARRAY_IMPL_HPP

#include <initializer_list>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include "../iterator.hpp"

namespace riot
{

/**
 * @brief C++ wrapper for c-arrays. Heavily inspired by the std::array.
 */
template <typename T, std::size_t arraySize>
class Array
{
public:
    // Member Types
    typedef T ValueType;
    typedef T& Reference;
    typedef const T& ConstReference;
    typedef T* Pointer;
    typedef const T* ConstPointer;
    typedef std::size_t SizeType;
    typedef SequenceIterator<T> Iterator;
    typedef SequenceIterator<const T> ConstIterator;
    typedef BackwardIterator<Iterator> ReverseIterator;
    typedef BackwardIterator<ConstIterator> ConstReverseIterator;

    /**
     * @brief Default Constructor. Warning: No Array initialization.
     */
    Array()
    {
    }

    /**
     * @brief Constructor: Initialize every Array element with @p initValue.
     * @param[in] initValue   Ref to the value used for initialization.
     */
    Array(ConstReference initValue)
    {
        this->fill(initValue);
    }

    /**
     * @brief Constructor: Initialize Array with initializer_list. Copies
     *        up size() elements from @p li into constructed array.
     * @param[in] li   Refernce to init list used for initialization.
     */
    Array(const std::initializer_list<ValueType>& li)
    {
      SizeType size = (this->size() < li.size()) ? this->size() : li.size();
      for (SizeType i = 0; i < size; ++i) {
          this->array_[i] = *(li.begin() + i);
      }
    }

    /**
     * @brief Copy-Constructor
     * @param[in] other   Array to copy.
     */
    Array(const Array& other)
    {
        memcpy(this->array_, other.array_, sizeof(this->array_));
    }

    /**
     * @brief Assignment operator.
     * @param[in] other   Reference to object that should be assigned.
     * @return            Reference to this object
     */
    auto operator = (const Array& other) -> Array&
    {
        if (this != &other) {
            memcpy(this->array_, other.array_, sizeof(this->array_));
        }
        return *this;
    }

    /**
     * @brief Operator [], mutable access to a arrays element.
     * @note Like a normal c array, [] performs no boundry checks.
     * @param[in] pos   Index to the referenced element.
     * @returns         Reference to the @p pos-th element.
     */
    auto operator [] (SizeType pos) -> Reference
    {
        return this->array_[pos];
    }

    /**
     * @brief Operator [], unmutable access to a arrays element.
     * @note Like a normal c array, [] performs no boundry checks.
     * @param[in] pos   Index to the referenced element.
     * @returns         Const reference to the @p pos-th element.
     */
    auto operator [] (SizeType pos) const -> ConstReference
    {
        return this->array_[pos];
    }

    /**
     * @brief Mutable element access with boundry check.
     * @param[in] pos    Index to the referenced element.
     * @param[out] err   Error field. Zero if @pos was withing boundries.
     *                   -EINVAL if not.
     * @returns          Reference to the @p pos-th element, if @p pos was
     *                   within boundries. If not, a reference to the
     *                   first element is returned.
     */
    auto at(SizeType pos, int& err) -> Reference
    {
        if (pos < arraySize) {
            err = 0;
            return this->array_[pos];
        }
        err = -EINVAL;
        return this->array_[0];
    }

    /**
     * @brief Unmutable element access with boundry check.
     * @param[in] pos    Index to the referenced element.
     * @param[out] err   Error field. Zero if @pos was withing boundries.
     *                   -EINVAL if not.
     * @returns          Const reference to the @p pos-th element, if @p pos
     *                   was within boundries. If not, a const reference to
     *                   the first element is returned.
     */
    auto at(SizeType pos, int& err) const -> ConstReference
    {
        if (pos < arraySize) {
            err = 0;
            return this->array_[pos];
        }
        err = -EINVAL;
        return this->array_[0];
    }

    /**
     * @brief Get mutable pointer to the underlaying c-array.
     * @returns   pointer to the wrapped c-array.
     */
    auto data() -> Pointer
    {
        return this->array_;
    }

    /**
     * @brief Get unmutable pointer to the underlaying c-array.
     * @returns   const pointer to the wrapped c-array.
     */
    auto data() const -> ConstPointer
    {
        return this->array_;
    }

    /**
     * @brief Returns a forward iterator pointing to the first element
     *        in the array container.
     * @returns   Iterator to [0].
     */
    auto begin() -> Iterator
    {
        return Iterator(this->array_);
    }

    /**
     * @brief Returns a forward iterator pointing to the past-the-end element
     *        in the array container. Do not dereference.
     * @returns   Iterator to [size()].
     */
    auto end() -> Iterator
    {
        return Iterator(this->array_ + arraySize);
    }

    /**
     * @brief Returns a reverse iterator pointing to the last element
     *        in the array container.
     * @returns   Iterator to [size() - 1].
     */
    auto rbegin() -> ReverseIterator
    {
        return ReverseIterator(Iterator(this->array_ + arraySize - 1));
    }

    /**
     * @brief Returns a reverse iterator pointing to the past-the-first element
     *        in the array container. Do not dereference.
     * @returns   Iterator to [-1].
     */
    auto rend() -> ReverseIterator
    {
        return ReverseIterator(Iterator(this->array_ - 1));
    }

    /**
     * @brief Returns a const forward iterator pointing to the first element
     *        in the array container.
     * @returns   Const iterator to [0].
     */
    auto cbegin() const -> ConstIterator
    {
        return ConstIterator(this->array_);
    }

    /**
     * @brief Returns a const forward iterator pointing to the past-the-end
     *        element in the array container. Do not dereference.
     * @returns   Const iterator to [size()].
     */
    auto cend() const -> ConstIterator
    {
        return ConstIterator(this->array_ + arraySize);
    }

    /**
     * @brief Returns a const reverse iterator pointing to the last element
     *        in the array container.
     * @returns   Const iterator to [size() - 1].
     */
    auto crbegin() const -> ConstReverseIterator
    {
        return ConstReverseIterator(ConstIterator(this->array_ + arraySize - 1));
    }

    /**
     * @brief Returns a const reverse iterator pointing to the past-the-first
     *        element in the array container. Do not dereference.
     * @returns   Const iterator to [-1].
     */
    auto crend() const -> ConstReverseIterator
    {
        return ConstReverseIterator(ConstIterator(this->array_ - 1));
    }

   /**
    * @brief Returns the number of elements in the array container.
    *        Always the value of the second template parameter.
    * @returns   Array size.
    */
    auto size() const -> SizeType
    {
        return arraySize;
    }

    /**
     * @brief Sets val as the value for all the elements in the array object.
     * @param[in] val   Reference to the new value for all elements.
     */
    auto fill(ConstReference val) -> void
    {
        for (SizeType i = 0; i < arraySize; ++i) {
            array_[i] = val;
        }
    }

    /**
     * @brief Swaps content with another array of same type and size.
     * @param[in,out] other   Array to swap elements with.
     */
    auto swap(Array& other) -> void
    {
        for (SizeType i = 0; i < this->size(); ++i) {
            ValueType tmp = array_[i];
            array_[i] = other[i];
            other[i] = tmp;
        }
    }

    private:
        ValueType array_[arraySize];   /**< Internal c-array */
};

/**
 * @brief == operator on arrays of the same type and length.
 * @param[in] lhs   Leftside of the operator.
 * @param[in] rhs   Rightside of the operator.
 * @returns         true if lhs and rhs internal arrays contain equal bytes.
 */
template <typename T, std::size_t arraySize>
auto operator == (const Array<T, arraySize>& lhs,
                  const Array<T, arraySize>& rhs) -> bool
{
    if (&lhs == &rhs) {
        return true;
    }
    return (0 == memcmp(lhs.data(), rhs.data(), arraySize * sizeof(T)));
}

/**
 * @brief != operator on arrays of the same type and length.
 * @param[in] lhs   Leftside of the operator.
 * @param[in] rhs   Rightside of the operator.
 * @returns         true if lhs and rhs internal arrays contents differ.
 */
template <typename T, std::size_t arraySize>
auto operator != (const Array<T, arraySize>& lhs,
                  const Array<T, arraySize>& rhs) -> bool
{
    return !(lhs == rhs);
}

} // namespace riot
#endif //ARRAY_IMPL_HPP
