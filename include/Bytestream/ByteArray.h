/**
 * @file ByteArray.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Byte array that owns its memory
 * @version 0.1
 * @date 2022-06-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef BYTESTREAM_BYTEARRAY_H
#define BYTESTREAM_BYTEARRAY_H

#include "Bytestream/ByteArrayView.h"
#include "Bytestream/ConstByteArrayView.h"

namespace Bytestream
{
    class ByteArray : public ByteArrayView
    {
    public:
        /// Create an invalid byte array wrapping a nullptr
        ByteArray();
        /// Create a byte array which holds n uninitialized bytes
        ByteArray(std::size_t n);
        /// Create a byte array holding n copies of value
        ByteArray(std::size_t n, std::byte value);
        /// Create a byte array containing a copy of the provided data
        ByteArray(const void *data, std::size_t n);
        /// Create a byte array containing a copy of the provided array
        ByteArray(const ConstByteArrayView &array);
        /// Move constructor
        ByteArray(ByteArray &&other);
        ~ByteArray();

        /// Create a byte array from a hex encoded string
        static ByteArray fromHex(const std::string &hex);

        /// Create a byte array from a binary encoded string
        static ByteArray fromBinary(const std::string &binary);

    private:
        bool m_owning{true};
    };
} // namespace Bytestream

#endif //> !BYTESTREAM_BYTEARRAY_H