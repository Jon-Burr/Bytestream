/// @file string.hxx
/// @brief Helper functions for converting to and from strings

#ifndef BYTESTREAM_STRING_HXX
#define BYTESTREAM_STRING_HXX

#include <cstddef>
#include <string>

namespace Bytestream {
    /// @brief Convert a single byte to its binary form
    std::string toBinary(std::byte b);
} // namespace Bytestream

#endif //> !BYTESTREAM_STRING_HXX