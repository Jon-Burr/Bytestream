#include "Bytestream/string.hxx"
#include <charconv>
#include <climits>
#include <cstring>
#include <system_error>

namespace Bytestream {
    std::string toBinary(std::byte b) {
        std::string str(CHAR_BIT, '0');
        std::to_chars_result result =
                std::to_chars(str.data(), str.data() + CHAR_BIT, std::to_integer<uint8_t>(b), 2);
        if (result.ec != std::errc())
            throw std::system_error(std::make_error_code(result.ec));
        std::size_t nChars = std::distance(str.data(), result.ptr);
        std::memmove(str.data() + CHAR_BIT - nChars, str.data(), nChars);
        std::memset(str.data(), '0', CHAR_BIT - nChars);
        return str;
    }

    std::string toHex(std::byte b) {
        std::string str(CHAR_BIT / 4, '0');
        std::to_chars_result result = std::to_chars(
                str.data(), str.data() + CHAR_BIT / 4, std::to_integer<uint8_t>(b), 16);
        if (result.ec != std::errc())
            throw std::system_error(std::make_error_code(result.ec));
        std::size_t nChars = std::distance(str.data(), result.ptr);
        std::memmove(str.data() + CHAR_BIT / 4 - nChars, str.data(), nChars);
        std::memset(str.data(), '0', CHAR_BIT / 4 - nChars);
        return str;
    }
} // namespace Bytestream