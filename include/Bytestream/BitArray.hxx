/**
 * @file BitArray.hxx
 * @brief Owning view over an array of bits
 */

#ifndef BYTESTREAM_BITARRAY_HXX
#define BYTESTREAM_BITARRAY_HXX

#include "Bytestream/BitArrayView.hxx"
#include "Bytestream/Endian.hxx"

namespace Bytestream {
    /// @brief Array view class which owns its own memory
    /// @tparam E Endianness used to interpret the underlying memory
    ///
    /// BitArrays always store their data with 0 offset
    template <Endian E> class BitArray : public BitArrayView<E> {
    public:
        /// @brief Construct a bit array from a string containing binary data
        /// @param data The data, assumed to presented in big-endian format
        static BitArray fromBinary(const std::string &data);

        /// @brief Construct a bit array from a string containing hexadecimal data
        /// @param data The data, assumed to presented in big-endian format
        static BitArray fromHex(const std::string &data);
        /// Create a byte array which holds n uninitialized bits
        explicit BitArray(std::size_t n);
        /// Create a bit array holding n copies of value
        explicit BitArray(std::size_t n, bool value);
        /// Create a bit array containing a copy of the provided data
        explicit BitArray(const void *data, std::size_t n);
        /// Create a byte array containing a copy of the provided array
        template <Endian E2> explicit BitArray(const ConstBitArrayView<E2> &array);
        /// Move constructor
        BitArray(BitArray &&other);
        ~BitArray();

        using BitArrayView<E>::operator=;

    private:
        bool m_owning{true};
    };

    template <typename T, Endian E = Endian::Native> BitArray<E> copyOf(const T &value) {
        return BitArray<E>(&value, sizeof(T) * CHAR_BIT);
    }
} // namespace Bytestream

#endif //> !BYTESTREAM_BITARRAY_HXX