/**
 * @file ReadBytestreamHelpers.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Helper classes for reading data from bytestreams
 * @version 0.1
 * @date 2022-06-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef BYTESTREAM_READBYTESTREAMHELPERS_H
#define BYTESTREAM_READBYTESTREAMHELPERS_H

#include "Bytestream/ConstByteArrayView.h"
#include "Bytestream/Endian.h"
#include "Bytestream/ReadBytestream.h"
#include "Bytestream/Utils.h"

#include <type_traits>

namespace Bytestream
{

    /**
     * @brief Holds information on how to interpret a piece of memory
     *
     * The base reader class does not own the memory that it points to so it
     * must not outlive the object which does
     */
    class Reader
    {
        friend ReadBytestream &operator>>(ReadBytestream &, Reader);

    public:
        /**
         * @brief Construct a new Reader object
         *
         * @param target The target memory location
         * @param nBits The number of bits to read from the stream
         * @param sourceEndianness The endianness of the data in the stream
         */
        Reader(void *target, std::size_t nBits,
               Endian sourceEndianness = Endian::Big);

    protected:
        std::byte *m_target;
        std::size_t m_nBits;
        Endian m_endianness;
    };

    /**
     * @brief Create a reader that reads into an existing value
     *
     * @tparam T The type being read into
     * @param value The value to read into
     * @param nBits The number of bits to read
     * @param sourceEndianness The endianness of the data in the source stream
     */
    template <typename T>
    std::enable_if_t<is_uint_v<T>, Reader>
    makeReader(T &value, std::size_t nBits,
               Endian sourceEndianness = Endian::Big);

    /**
     * @brief Create a reader that reads into an existing value
     *
     * This will read the maximum number of  bits (sizeof(T) * CHAR_BIT) from
     * the stream
     *
     * @tparam T The type being read into
     * @param value The value to read into
     * @param sourceEndianness The endianness of the data in the source stream
     */
    template <typename T>
    std::enable_if_t<is_uint_v<T>, Reader>
    makeReader(T &value, Endian sourceEndianness);

    /// Read information from the stream into the location specified by the
    /// reader
    ReadBytestream &operator>>(ReadBytestream &stream, Reader reader);

    /**
     * @brief Helper struct to skip over a certain number of bits in a stream
     */
    struct SkipBits
    {
        SkipBits(std::size_t nBits);
        std::size_t nBits;
    };

    /// Skip the number of bits specified by the value
    ReadBytestream &operator>>(ReadBytestream &stream, SkipBits skip);

    /**
     * @brief Helper class which holds an unsigned integer value to be read from
     * the stream
     *
     * @tparam T The uint type to be read into
     */
    template <typename T>
    class UIntHandle : public Reader
    {
        static_assert(is_uint_v<T>, "Only valid for unsigned integers");

    public:
        /**
         * @brief Construct a new UIntHandle object
         *
         * @param nBits The number of bits to read from the stream
         * @param sourceEndianness The endianness of the source data
         */
        UIntHandle(std::size_t nBits = sizeof(T) * CHAR_BIT,
                   Endian sourceEndianness = Endian::Big);
        operator T() const { return m_value; }
        T value() const { return m_value; }

    private:
        T m_value{0};
    };

    /**
     * @brief Helper class which holds a float to be read from a stream
     *
     * @tparam F The floating point type
     * @tparam T The underlying unsigned integer type
     */
    template <typename F = float, typename T = uint32_t>
    class FloatHandle : public Reader
    {
    public:
        /**
         * @brief Construct a new Float Handle object
         *
         * @param conversion Conversion factor to convert from the uint type to
         * the floating point type
         * @param nBits The number of bits to read
         * @param sourceEndianness The endianness of the data in the source
         * stream
         */
        FloatHandle(F conversion, std::size_t nBits = sizeof(T) * CHAR_BIT,
                    Endian sourceEndianness = Endian::Big);
        operator F() const { return value(); }
        F value() const { return m_value * m_conversion; }

        T rawValue() const { return m_value; }

    private:
        F m_conversion;
        T m_value;
    };

    /**
     * @brief Handle which performs basic data validation
     *
     * The handle has an array of bytes which it expects to see and it compares
     * the value that it actually receives to this.
     */
    class ExpectsHandle : public Reader
    {
    public:
        /**
         * @brief Construct a new Expects Handle object
         *
         * If nBits % CHAR_BIT != 0 you are responsible for ensuring that the
         * leftmost bits of the expected array are 0
         *
         * @param expected The start of the expected byte array
         * @param nBits The number of bits to read from the target stream
         */
        ExpectsHandle(const void *expected, std::size_t nBits);

        /**
         * @brief Construct a new Expects Handle object
         *
         * The number of bits will be the number contained within the view.
         *
         * @param view View containing the expected data
         */
        ExpectsHandle(const ConstByteArrayView &view);
        ~ExpectsHandle();

        /// Return whether the read data was correct
        operator bool() const { return good(); }
        /// Return whether the read data was correct
        bool good() const;
        /// Throw an error if the read data was incorrect
        void throwIfBad() const;

        std::size_t nBits() const;
        const void *expected() const;
        const void *received() const;

    private:
        const void *m_expected;
    };

    /**
     * @brief Read the specified number of bits and check that they are as
     * expected
     *
     * Sets the fail bit on the stream if the bins are not as expected
     *
     */
    ReadBytestream &operator>>(ReadBytestream &stream, ExpectsHandle &reader);

} // namespace Bytestream

#include "Bytestream/ReadBytestreamHelpers.icc"
#endif