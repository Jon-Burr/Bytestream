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
#include "Bytestream/Endian.h"
#include "Bytestream/Utils.h"
#include <bitset>
#include <ios>
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

        /// Is the stream in a good state?
        bool good() const { return m_state == std::ios_base::goodbit; }
        /// Has the stream exhausted its input?
        bool eof() const { return m_state & std::ios_base::eofbit; }
        /// Did the read operation fail somehow?
        bool fail() const { return m_state & std::ios_base::failbit; }
        /// Currently unused by internal functions
        bool bad() const { return m_state & std::ios_base::badbit; }
        /// Read the full current state
        std::ios_base::iostate rdstate() const { return m_state; }
        /// Set the state (ORed to the existing state)
        void setstate(std::ios_base::iostate state) { m_state |= state; }
        /// Clear the state to the provided value
        void clear(std::ios_base::iostate state = std::ios_base::goodbit) { m_state = state; }
        /// Set the EOF bit
        void seteof() { setstate(std::ios_base::eofbit); }
        /// Set the fail bit
        void setfail() { setstate(std::ios_base::failbit); }
        /// Set the bad bit
        void setbad() { setstate(std::ios_base::badbit); }

        bool onByteBoundary() const;
        /**
         * @brief Read a number of bits into a target location
         *
         * @param target The target memory location
         * @param nBits The number of bits to read
         * @param sourceEndianness The endianness of the data in the source
         * stream
         */
        void readBitsInto(std::byte *target, std::size_t nBits, Endian sourceEndianness);
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
        std::enable_if_t<is_uint_v<T>, void> readBits(
                T &target, std::size_t nBits = sizeof(T) * CHAR_BIT,
                Endian sourceEndianness = Endian::Big);

    private:
        ConstByteArrayView m_view;
        std::size_t m_pos{0};
        std::ios_base::iostate m_state{std::ios_base::goodbit};
    };

    template <typename T>
    std::enable_if_t<is_uint_v<T>, ReadBytestream &> operator>>(ReadBytestream &stream, T &value);

    template <std::size_t N>
    ReadBytestream &operator>>(ReadBytestream &stream, std::bitset<N> &value);

} // namespace Bytestream

#include "Bytestream/ReadBytestream.icc"
#endif //> !BYTESTREAM_READBYTESTREAM_H