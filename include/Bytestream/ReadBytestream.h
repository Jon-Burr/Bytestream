/**
 * @file ReadBytestream.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Read data from a byte array using a stream-like formalism
 * @version 0.1
 * @date 2022-06-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef BYTESTREAM_READBYTESTREAM_H
#define BYTESTREAM_READBYTESTREAM_H

#include "Bytestream/ConstByteArrayView.h"
#include "Bytestream/Utils.h"

#include <bit>
#include <bitset>
#include <type_traits>
namespace Bytestream {
    class ReadBytestream {
    public:
        ReadBytestream(const ConstByteArrayView &view);
        std::size_t position() const;
        std::size_t size() const;
        std::size_t remaining() const;
        ConstByteArrayView data() const { return m_view; }
        bool exhausted() const;
        void advance(std::size_t n);
        void rewind(std::size_t n);
        void seek(std::size_t pos);
        void reset();
        bool onByteBoundary() const;
        /**
         * @brief Read a number of bits into a target location
         *
         * @param target The target memory location
         * @param nBits The number of bits to read
         * @param sourceEndianness The endianness of the data in the source
         * stream
         */
        void readBitsInto(std::byte *target, std::size_t nBits,
                          std::endian sourceEndianness);
        /**
         * @brief Read out a number of unformatted bytes into a view
         *
         * @param nBytes The number of bytes to read
         * @pre onByteBoundary() == true [throw std::invalid_argument]
         */
        ConstByteArrayView readUnformattedBytes(std::size_t nBytes);

        /**
         * @brief Read a number of bits into a target unsigned integer
         *
         * @tparam T The uint type to read into
         * @param target The target unsigned integer
         * @param nBits The number of bits to read
         * @param sourceEndianness The endianness of the data in the stream
         */
        template <typename T>
        std::enable_if_t<is_uint_v<T>, void>
        readBits(T &target, std::size_t nBits = sizeof(T) * CHAR_BIT,
                 std::endian sourceEndianness = std::endian::big);

    private:
        ConstByteArrayView m_view;
        std::size_t m_pos{0};
    };

    template <typename T>
    std::enable_if_t<is_uint_v<T>, ReadBytestream &>
    operator>>(ReadBytestream &stream, T &value);

    template <std::size_t N>
    ReadBytestream &operator>>(ReadBytestream &stream, std::bitset<N> &value);

} // namespace Bytestream

#include "Bytestream/ReadBytestream.icc"
#endif //> !BYTESTREAM_READBYTESTREAM_H