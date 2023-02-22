/**
 * @file AlignedByteIterator.hxx
 * @brief Utility iterator to iterate over the bytes of offset arrays as if they were not offset
 */

#ifndef BYTESTREAM_ALIGNEDBYTEITERATOR_HXX
#define BYTESTREAM_ALIGNEDBYTEITERATOR_HXX

#include "Bytestream/ByteProxy.hxx"
#include <iterator>

namespace Bytestream {

    /// @brief Iterator over a bit array that returns the represented bytes without offset
    /// @tparam Iterator The underlying iterator type
    ///
    /// The iterator runs from the LSB to MSB of the viewed data. The bytes are returned as proxy
    /// objects which behave as if the underlying memory had a bit offset of 0. If the number of
    /// bits in the underlying array is not a multiple of CHAR_BIT then the final byte is truncated
    /// with zeroes returned in place of the out of range bits.
    ///
    /// If the underlying iterator type is mutable then the proxy allows assigning values to the
    /// underlying array. These also behave as though the underlying array had an offset of 0 and
    /// any bits out of range of the array will not be changed
    ///
    /// This class can only be used to iterate forwards through an array, it cannot act as a
    /// reverse iterator
    template <typename Iterator> class AlignedByteIterator {
    public:
        /// @brief The value returned by dereferencing this iterator
        using value_type = detail::iterator_proxy_t<Iterator>;
        /// @brief The value returned by dereferencing this iterator
        using reference = value_type;
        /// @brief Cannot use pointer access on this iterator
        using pointer = void;
        /// @brief Difference between two iterators
        using difference_type = std::ptrdiff_t;
        /// @brief The type of iterator
        using iterator_category = std::input_iterator_tag;

        /// @brief Create the iterator
        /// @param itr The underlying iterator over bytes
        /// @param offset The offset of the underlying array
        /// @param size The number of bits in the underlying array
        AlignedByteIterator(Iterator itr, uint8_t offset, std::size_t size);
        /// @brief Copy constructor
        /// @param other The iterator to copy
        AlignedByteIterator(const AlignedByteIterator &other) = default;

        /// @brief Two iterators compare equal if they point to the same point in the same
        /// underlying array
        bool operator==(const AlignedByteIterator &other) const;
        /// @brief Two iterators compare equal if they point to the same point in the same
        /// underlying array
        bool operator!=(const AlignedByteIterator &other) const;

        /// @brief Dereference the iteratorr
        /// @return The byte proxy type
        reference operator*() const;
        /// @brief (pre) increment the iterator
        AlignedByteIterator &operator++();
        /// @brief (post) increment the iterator
        AlignedByteIterator operator++(int);

    private:
        Iterator m_itr;
        uint8_t m_offset;
        std::size_t m_size;
    };
} // namespace Bytestream

#endif //> !BYTESTREAM_ALIGNEDBYTEITERATOR_HXX