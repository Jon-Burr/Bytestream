/**
 * @file StringUtils.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Utilities to convert between string and memory representations of data
 * @version 0.1
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef BYTESTREAM_STRINGUTILS_H
#define BYTESTREAM_STRINGUTILS_H

#include <cstddef>
#include <string>
#include <vector>

namespace Bytestream
{
    std::vector<std::byte> fromHex(const std::string &str);
    std::vector<std::byte> fromBinary(const std::string &str);

    std::string toHex(const void *data, std::size_t n);
    std::string toBinary(const void *data, std::size_t n);

    template <typename T> std::string toHex(const T &value)
    {
        return toHex(&value, sizeof(T));
    }

    template <typename T> std::string toBinary(const T &value)
    {
        return toBinary(&value, sizeof(T));
    }
} // namespace Bytestream

#endif //> !BYTESTREAM_STRINGUTILS_H