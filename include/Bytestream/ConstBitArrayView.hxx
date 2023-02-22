/**
 * @file ConstBitArrayView.hxx
 *
 * Provide a const view over a selection of bits
 */

#ifndef BYTESTREAM_CONSTBITARRAYVIEW_HXX
#define BYTESTREAM_CONSTBITARRAYVIEW_HXX

#include "Bytestream/AlignedByteIterator.hxx"
#include "Bytestream/ArrayForward.hxx"
#include "Bytestream/ByteIterator.hxx"
#include "Bytestream/ConstBitIterator.hxx"
#include "Bytestream/Endian.hxx"
#include "Bytestream/utils.hxx"

#include <climits>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <type_traits>

namespace Bytestream {
    /// @brief A view over a range of bits in memory
    /// @tparam E The endianness of the array
    template <Endian E> class ConstBitArrayView {
    public:
        /// @brief Iterator over individual bits (from least to most significant)
        using iterator = ConstBitIterator<E, true>;
        /// @brief Iterator over individual bits (from most to least significant)
        using reverse_iterator = ConstBitIterator<E, false>;

        /// @brief Create the view
        /// @param data Pointer to the start of the data in memory
        /// @param nBits The number of bits in the view
        /// @param offset The offset within the first byte
        ///
        /// offset must be less than CHAR_BIT
        ConstBitArrayView(const void *data, std::size_t nBits, uint8_t offset);

        /// @brief Create a const bit view of a value
        /// @tparam T The type of the value
        /// @param value The value
        /// @param pos Start the view at this position
        /// @param nBits Hold at most this number of bits
        template <typename T>
        static ConstBitArrayView viewOf(
                const T &value, std::size_t pos, std::size_t nBits = SIZE_MAX);

        /// @brief Create a const bit view of a value
        /// @tparam T The type of the value
        /// @param value The value
        template <typename T> static ConstBitArrayView viewOf(const T &value);

        /// @brief Access the value of the bit at idx
        bool operator[](std::size_t idx) const;
        /// @brief Access the value of the bit at idx with a range check
        bool at(std::size_t idx) const;

        /// @brief The number of bits in the  view
        std::size_t size() const { return m_nBits; }

        /// @brief The bit offset in the LSB
        uint8_t offset() const { return m_offset; }

        /// @brief Create a sub view over the same data
        /// @param pos Start from this position (measured from the LSB)
        /// @param nBits At most this many bits. Will not overflow the available storage
        ConstBitArrayView sub(std::size_t pos, std::size_t nBits = SIZE_MAX) const;

        /// @brief Create a copy of this view
        ///
        /// The copied array will be aligned (offset = 0)
        BitArray<E> copy() const;

        /// @brief Produce a copy of this data with the endianness swapped
        ///
        /// The interpretation of the data will be the same, but the underlying representation will
        /// be shifted
        BitArray<swapped_v<E>> swapEndianness() const;

        /// @brief Get the data held in this view as specified type
        /// @tparam T The type to interpret the data as
        template <typename T>
        std::enable_if_t<std::is_default_constructible_v<T>, T> asType() const;

        /// @brief Convert to a binary representation
        ///
        /// The LSB is printed in the rightmost position
        std::string toBinary() const;

        /// @brief Convert to a hexadecimal representation
        ///
        /// The LSB is printed in the rightmost position
        std::string toHex() const;

        /// @brief Pointer to the start byte in memory
        const std::byte *data() const { return m_data; }
        /// @brief The number of bytes covered in memory
        std::size_t nCoveredBytes() const { return nBytesToHold(m_nBits + m_offset); }

        /// @brief Pointer to the byte containing the least significant bit
        const std::byte *lsb() const;

        /// @brief Pointer to the byte containing the most significant bit
        const std::byte *msb() const;

        /// @brief Iterator from the least significant bit
        iterator begin() const { return iterator(byte_begin(), m_offset); }
        /// @brief Iterator past the most significant bit
        iterator end() const { return begin() + size(); }
        /// @brief Iterator from the most significant bit
        reverse_iterator rbegin() const { return reverse_iterator(byte_rbegin(), msbPad()); }
        /// @brief Iterator past the least significant bit
        reverse_iterator rend() const { return rbegin() + size(); }

        /// The byte iterators iterate over whole bytes ignoring offsets
        /// @brief Iterator starting from the LSB
        ConstByteIterator<E> byte_begin() const;
        /// @brief Iterator after the MSB
        ConstByteIterator<E> byte_end() const;
        /// @brief Iterator starting from the MSB
        ReverseConstByteIterator<E> byte_rbegin() const;
        /// @brief Iterator after the LSB
        ReverseConstByteIterator<E> byte_rend() const;

        /// @brief Mutable iterator to the LSB that behaves as though the offset is 0
        AlignedByteIterator<ConstByteIterator<E>> aligned_byte_begin() const;
        /// @brief End iterator for the aligned bytes
        AlignedByteIterator<ConstByteIterator<E>> aligned_byte_end() const;

    protected:
        /// @brief The number of bits to the left in the MSB
        uint8_t msbPad() const;
        /// @brief Start of the data in memory
        const std::byte *const m_data;
        /// @brief The number of bits in the array
        const std::size_t m_nBits;
        /// @brief The offset of the first bit in the first byte
        const uint8_t m_offset;
    }; //> end class ConstBitArrayView

    /// @brief Stream operator for a byte arrary prints the binary representation
    template <Endian E> std::ostream &operator<<(std::ostream &os, const ConstBitArrayView<E> &v) {
        return os << v.toBinary();
    }

    /// @brief Create a const bit array view over the provided object
    /// @tparam E The endianness to use to interpret the memory
    /// @tparam T The type of object
    /// @param value The value to create the view over
    template <Endian E, typename T> ConstBitArrayView<E> viewOf(const T &value) {
        ConstBitArrayView<E>::viewOf(value);
    }

    /// @brief Create a const bit array view over the provided object
    /// @tparam T The type of object
    /// @param value The value to create the view over
    ///
    /// The returned view will use the native endianness
    template <typename T> ConstBitArrayView<Endian::Native> viewOf(const T &value) {
        return ConstBitArrayView<Endian::Native>::viewOf(value);
    }
} // namespace Bytestream

#include "Bytestream/ConstBitArrayView.ixx"
#endif //> !BYTESTREAM_CONSTBITARRAYVIEW_HXX