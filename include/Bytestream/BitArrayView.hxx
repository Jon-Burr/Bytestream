/**
 * @file ConstBitArrayView.hxx
 *
 * Provide a mutable view over a selection of bits
 */

#ifndef BYTESTREAM_BITARRAYVIEW_HXX
#define BYTESTREAM_BITARRAYVIEW_HXX

#include "Bytestream/BitProxy.hxx"
#include "Bytestream/ConstBitArrayView.hxx"

// NB: The const casts used here are definitely safe because this class only ever passes a
// non-const pointer to the base class

namespace Bytestream {
    /// @brief A mutable view over a range of bits in memory
    /// @tparam E The endianness of the underlying array
    template <Endian E> class BitArrayView : public ConstBitArrayView<E> {
    public:
        // @brief Create the view
        /// @param data Pointer to the start of the data in memory
        /// @param nBits The number of bits in the view
        /// @param offset The offset within the first byte
        ///
        /// offset must be less than CHAR_BIT
        BitArrayView(void *data, std::size_t nBits, uint8_t offset);

        /// @brief Create a const bit view of a value
        /// @tparam T The type of the value
        /// @param value The value
        /// @param pos Start the view at this position
        /// @param nBits Hold at most this number of bits
        template <typename T>
        static BitArrayView viewOf(T &value, std::size_t pos, std::size_t nBits = SIZE_MAX);

        /// @brief Create a const bit view of a value
        /// @tparam T The type of the value
        /// @param value The value
        template <typename T> static BitArrayView viewOf(T &value);

        /// @brief Set the viewed area with the data held in the other view
        /// @tparam E2 The endianness of the other arry
        ///
        /// If the other view is shorter than this one, extra bytes are zeroed. If this view is
        /// shorter than the other then the value is truncated. The endianness of the other array
        /// does not matter for the interpretation of the data, i.e. data from the LSB of the other
        /// will be copied to the LSB of this
        template <Endian E2> BitArrayView<E> &operator=(const ConstBitArrayView<E2> &other);

        /// @brief Get a mutable proxy for the specified bit
        /// @param idx The bit index (counted from the least significant)
        ///
        /// This version is not range-checked
        BitProxy operator[](std::size_t idx);
        using ConstBitArrayView<E>::operator[];
        /// @brief Get a mutable proxy for the specified bit
        /// @param idx The bit index (counted from the least significant)
        BitProxy at(std::size_t idx);
        using ConstBitArrayView<E>::at;

        /// @brief Set contained memory from the provided value
        /// @tparam T The type of provided value
        /// @tparam E2 How to interpret the endianness of the provided value
        /// @param value The provided value
        ///
        /// Bits are copied from the LSB of the provided value to the LSB of this. If the value has
        /// more bits than this view then the extra bits are ignored. If it has fewer then the extra
        /// bits in this view are zeroed.
        template <typename T, Endian E2 = Endian::Native> void setFrom(const T &value);

        /// @brief Create a sub view over the same data
        /// @param pos Start from this position (measured from the LSB)
        /// @param nBits At most this many bits. Will not overflow the available storage
        BitArrayView sub(std::size_t pos, std::size_t nBits = SIZE_MAX);
        using ConstBitArrayView<E>::sub;

        /// @brief Pointer to the start byte in memory
        std::byte *data() { return const_cast<std::byte *>(ConstBitArrayView<E>::data()); }
        using ConstBitArrayView<E>::data;

        /// @brief Pointer to the byte containing the least significant bit
        std::byte *lsb() { return const_cast<std::byte *>(ConstBitArrayView<E>::lsb()); }

        /// @brief Pointer to the byte containing the most significant bit
        std::byte *msb() { return const_cast<std::byte *>(ConstBitArrayView<E>::msb()); }

        /// The byte iterators iterate over whole bytes ignoring offsets
        /// @brief Iterator starting from the LSB
        ByteIterator<E> byte_begin();
        using ConstBitArrayView<E>::byte_begin;
        /// @brief Iterator after the MSB
        ByteIterator<E> byte_end();
        using ConstBitArrayView<E>::byte_end;
        /// @brief Iterator starting from the MSB
        ReverseByteIterator<E> byte_rbegin();
        using ConstBitArrayView<E>::byte_rbegin;
        /// @brief Iterator after the LSB
        ReverseByteIterator<E> byte_rend();
        using ConstBitArrayView<E>::byte_rend;

        /// @brief Mutable iterator to the LSB that behaves as though the offset is 0
        AlignedByteIterator<ByteIterator<E>> aligned_byte_begin();
        using ConstBitArrayView<E>::aligned_byte_begin;
        /// @brief End iterator for the aligned bytes
        AlignedByteIterator<ByteIterator<E>> aligned_byte_end();
        using ConstBitArrayView<E>::aligned_byte_end;
    };

    /// @brief Create a mutable bit array view over the provided object
    /// @tparam E The endianness to use to interpret the memory
    /// @tparam T The type of object
    /// @param value The value to create the view over
    template <Endian E, typename T> ConstBitArrayView<E> viewOf(T &value) {
        BitArrayView<E>::viewOf(value);
    }

    /// @brief Create a mutable bit array view over the provided object
    /// @tparam T The type of object
    /// @param value The value to create the view over
    ///
    /// The returned view will use the native endianness
    template <typename T> BitArrayView<Endian::Native> viewOf(T &value) {
        return BitArrayView<Endian::Native>::viewOf(value);
    }
} // namespace Bytestream

#include "Bytestream/BitArrayView.ixx"

#endif //> !BYTESTREAM_BITARRAYVIEW_HXX