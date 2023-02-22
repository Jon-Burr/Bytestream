/// @file ConstBitIterator.hxx
/// @brief Iterator over individual (const) bits

#ifndef BYTESTREAM_CONSTBITITERATOR_HXX
#define BYTESTREAM_CONSTBITITERATOR_HXX

#include "Bytestream/ByteIterator.hxx"
#include "Bytestream/Endian.hxx"

#include <climits>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace Bytestream {

    template <Endian E, bool FORWARD> class ConstBitIterator;

    template <Endian E, bool FORWARD>
    bool operator==(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
    template <Endian E, bool FORWARD>
    bool operator!=(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
    template <Endian E, bool FORWARD>
    bool operator<(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
    template <Endian E, bool FORWARD>
    bool operator<=(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> operator+(
            ConstBitIterator<E, FORWARD> itr,
            typename ConstBitIterator<E, FORWARD>::difference_type step);
    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> operator-(
            ConstBitIterator<E, FORWARD> itr,
            typename ConstBitIterator<E, FORWARD>::difference_type step);
    template <Endian E, bool FORWARD>
    typename ConstBitIterator<E, FORWARD>::difference_type operator-(
            ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);

    /**
     * @brief Iterator over bits
     * @tparam E The endianness of the underlying stream
     * @tparam FORWARD The direction of the iterator
     */
    template <Endian E, bool FORWARD> class ConstBitIterator {
    public:
#ifndef NODOXYGEN
        using value_type = bool;
        using reference = bool;
        using pointer = void;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;
#endif

        /// @brief The type of byte iterator used
        using byte_itr_t =
                std::conditional_t<FORWARD, ConstByteIterator<E>, ReverseConstByteIterator<E>>;

        /// @brief Create the iterator
        /// @param itr Underlying byte iterator
        /// @param offset The offset within the byte
        ConstBitIterator(byte_itr_t itr, uint8_t offset);

        /// @brief Access the pointed to byte
        reference operator*() const;
        /// @brief (pre) increment the iterator
        ConstBitIterator &operator++();
        /// @brief (post) increment the iteratorr
        ConstBitIterator operator++(int);
        /// @brief Increment the iterator by the specified number of steps
        ConstBitIterator &operator+=(difference_type step);
        /// @brief Decrement the iterator by the specified number of steps
        ConstBitIterator &operator-=(difference_type step);
#ifndef NODOXYGEN
        friend bool operator==
                <>(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
        friend bool operator!=
                <>(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
        friend bool operator< <>(
                ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
        friend bool operator<=
                <>(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
        friend ConstBitIterator<E, FORWARD> operator+
                <>(ConstBitIterator<E, FORWARD> itr, difference_type step);
        friend ConstBitIterator<E, FORWARD> operator-
                <>(ConstBitIterator<E, FORWARD> itr, difference_type step);
        friend difference_type operator-
                <>(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs);
#endif
    private:
        byte_itr_t m_itr;
        uint8_t m_offset;
    }; //> end class ConstBitIterator

    /// @brief Compare equality between two bit iterators
    template <Endian E, bool FORWARD>
    bool operator==(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs) {
        return lhs.m_itr == rhs.m_itr && lhs.m_offset == rhs.m_offset;
    }

    /// @brief Compare inequality between two bit iterators
    template <Endian E, bool FORWARD>
    bool operator!=(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs) {
        return lhs.m_itr != rhs.m_itr || lhs.m_offset != rhs.m_offset;
    }

    /// @brief Less than comparison between two bit iterators
    template <Endian E, bool FORWARD>
    bool operator<(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs) {
        return lhs.m_itr <= rhs.m_itr && lhs.m_offset < rhs.m_offset;
    }

    /// @brief Less than or equal comparison between two bit iterators
    template <Endian E, bool FORWARD>
    bool operator<=(ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs) {
        return lhs.m_itr <= rhs.m_itr && lhs.m_offset <= rhs.m_offset;
    }

    /// @brief Advance a bit iterator by the specified number of bits
    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> operator+(
            ConstBitIterator<E, FORWARD> itr,
            typename ConstBitIterator<E, FORWARD>::difference_type step) {
        ConstBitIterator tmp = itr;
        return tmp += step;
    }

    /// @brief Decrement a bit iterator by the specified number of bits
    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> operator-(
            ConstBitIterator<E, FORWARD> itr,
            typename ConstBitIterator<E, FORWARD>::difference_type step) {
        ConstBitIterator tmp = itr;
        return tmp -= step;
    }

    /// @brief Number of steps between two bit iterators
    template <Endian E, bool FORWARD>
    typename ConstBitIterator<E, FORWARD>::difference_type operator-(
            ConstBitIterator<E, FORWARD> lhs, ConstBitIterator<E, FORWARD> rhs) {
        return CHAR_BIT * (lhs.m_itr - rhs.m_itr) + lhs.m_offset - rhs.m_offset;
    }
} // namespace Bytestream

#endif //>! BYTESTREAM_CONSTBITITERATOR_HXX