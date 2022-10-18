/**
 * @file ConstByteArrayView.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief A const view over a number of bytes
 * @version 0.1
 * @date 2022-06-18
 *
 * @copyright Copyright (c) 2022
 */

#ifndef BYTESTREAM_CONSTBYTEARRAYVIEW_H
#define BYTESTREAM_CONSTBYTEARRAYVIEW_H

#include "Bytestream/Endian.h"
#include "Bytestream/Utils.h"
#include <bitset>
#include <cstddef>
#include <iterator>
#include <ostream>
#include <type_traits>

namespace Bytestream
{
    /**
     * @brief A constant view over unformatted bytes
     *
     * The indexing of the array starts from the rightmost byte/bit
     */
    class ConstByteArrayView
    {
    public:
        using iterator = std::reverse_iterator<const std::byte *>;
        using reverse_iterator = const std::byte *;
        /**
         * @brief Construct a new Const Byte Array View object
         *
         * @param data The start of the data array in memory
         * @param nBytes The number of bytes in the data array
         */
        ConstByteArrayView(const void *data, std::size_t nBytes);

        /**
         * @brief Return a view over the data represented by the passed object
         */
        template <typename T>
        static ConstByteArrayView asView(const T &data);

        /// The number of bytes in the view
        std::size_t size() const { return m_nBytes; }

        /// Does this actually point to data?
        bool empty() const { return m_data == nullptr; }

        /// Implicit conversion to bool says whether the pointer is valid
        operator bool() const { return m_data; }

        /**
         * @brief The start of the underlying data array
         *
         * Remember that the underlying array will follow c indexing which is
         * inverted compared to ours.
         */
        const std::byte *data() const { return m_data; }

        /// Iterator to the rightmost byte
        iterator begin() const
        {
            return std::make_reverse_iterator(m_data + m_nBytes);
        }
        /// Iterator past the leftmost byte
        iterator end() const { return std::make_reverse_iterator(m_data); }
        /// Iterator to the leftmost byte
        reverse_iterator rbegin() const { return m_data; }
        /// Iterator past the rightmost byte
        reverse_iterator rend() const { return m_data + m_nBytes; }

        /// Access the specified byte (no bounds checking)
        std::byte operator[](std::size_t idx) const
        {
            return *(m_data + m_nBytes - idx - 1);
        }

        /**
         * @brief Access the specified byte with bounds checking
         *
         * @pre idx < size() [throw std::out_of_range]
         */
        std::byte at(std::size_t idx) const;

        /// Convert to a hex encoded string
        std::string toHex() const;
        /// Convert to a binary encoded string
        std::string toBinary() const;

        /// Get a view of a subset of this array
        ConstByteArrayView view(std::size_t idx, std::size_t nBytes) const;

        bool testBitsAny(std::size_t bitIdx, std::size_t nBits = 1) const;
        template <typename T>
        std::enable_if_t<is_uint_v<T>, bool> testBitsAny(
            std::size_t bitItx, std::size_t nBits, T mask,
            Endian maskEndianness = Endian::Native) const;
        bool testBitsAll(std::size_t bitIdx, std::size_t nBits = 1) const;
        template <typename T>
        std::enable_if_t<is_uint_v<T>, bool> testBitsAll(
            std::size_t bitItx, std::size_t nBits, T mask,
            Endian maskEndianness = Endian::Native) const;

        /**
         * @brief Read bits into the target value
         *
         * @tparam T The type of the target value
         * @param value The target value
         * @param bitPos The position (in bits) of the data in the array
         * @param nBits The number of bits to copy
         * @param sourceEndianness The endianness of the source stream
         * @param targetEndianness The endianness of the target memory
         *
         * NB: It only really makes sense to read truncated bytes from a
         * big-endian stream.
         */
        template <typename T>
        std::enable_if_t<is_uint_v<T>, void>
        readBits(T &value, std::size_t bitPos,
                 std::size_t nBits = sizeof(T) * CHAR_BIT,
                 Endian sourceEndianness = Endian::Big,
                 Endian targetEndianness = Endian::Native) const;

        /**
         * @brief Read bits into a value of the specified type
         *
         * @tparam T The type of the target value
         * @param bitPos The position (in bits) of the data in the array
         * @param nBits The number of bits to copy
         * @param sourceEndianness The endianness of the source stream
         * @param targetEndianness The endianness of the target memory
         *
         * NB: It only really makes sense to read truncated bytes from a
         * big-endian stream.
         */
        template <typename T = uint32_t>
        std::enable_if_t<is_uint_v<T>, T>
        readBits(std::size_t bitPos, std::size_t nBits = sizeof(T) * CHAR_BIT,
                 Endian sourceEndianness = Endian::Big,
                 Endian targetEndianness = Endian::Native) const;

        /**
         * @brief Read bytes into the target value
         *
         * @tparam T The type of the target value
         * @param value The target value
         * @param bytePos The position (in bytes) of the data in the array
         * @param nBytes The number of bytes to copy
         * @param sourceEndianness The endianness of the source stream
         * @param targetEndianness The endianness of the target memory
         */
        template <typename T>
        std::enable_if_t<is_uint_v<T>, void>
        readBytes(T &value, std::size_t bytePos, std::size_t nBytes = sizeof(T),
                  Endian sourceEndianness = Endian::Big,
                  Endian targetEndianness = Endian::Native) const;

        /**
         * @brief Read bytes into a value of the specified type
         *
         * @tparam T The type of the target value
         * @param bytePos The position (in bytes) of the data in the array
         * @param nBytes The number of bytes to copy
         * @param sourceEndianness The endianness of the source stream
         * @param targetEndianness The endianness of the target memory
         */
        template <typename T = uint32_t>
        std::enable_if_t<is_uint_v<T>, T>
        readBytes(std::size_t bytePos, std::size_t nBytes = sizeof(T),
                  Endian sourceEndianness = Endian::Big,
                  Endian targetEndianness = Endian::Native) const;

        /// Read a bitset from the specified position
        template <std::size_t N>
        std::bitset<N> readBitset(std::size_t bitPos);

        void
        readBitsInto(void *target, std::size_t bitPos, std::size_t nBits,
                     Endian sourceEndianness = Endian::Big,
                     Endian targetEndianness = Endian::Native) const;
        void
        readBytesInto(void *target, std::size_t bytePos, std::size_t nBytes,
                      Endian sourceEndianness = Endian::Big,
                      Endian targetEndianness = Endian::Native) const;

    private:
        const std::byte *const m_data;
        std::size_t m_nBytes;
    }; //> end class ConstByteArrayView

    bool operator==(const ConstByteArrayView &lhs, const ConstByteArrayView &rhs);
    bool operator!=(const ConstByteArrayView &lhs, const ConstByteArrayView &rhs);
    std::ostream &operator<<(std::ostream &os, const ConstByteArrayView &view);
} // namespace Bytestream

#include "Bytestream/ConstByteArrayView.icc"
#endif //> !BYTESTREAM_CONSTBYTEARRAYVIEW_H