#include "Bytestream/StringUtils.h"
#include <cctype>
#include <charconv>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <algorithm>

namespace
{
    std::vector<std::byte> fromStr(
        const std::string &data, std::size_t base, std::size_t nCharsPerByte)
    {
        // base is 2, 8 or 16
        std::vector<std::byte> bytes;
        bytes.reserve(data.size() / nCharsPerByte);

        // Current position in the string
        const char *ptr = data.data();
        // Number of characters remaining
        std::size_t n = data.size();
        while (n > 0)
        {
            // Skip over any space characters between bytes
            if (std::isspace(*ptr))
            {
                ++ptr;
                --n;
                continue;
            }
            if (n == 0)
                break;

            if (n < nCharsPerByte ||
                std::any_of(ptr, ptr + nCharsPerByte, [](unsigned char c)
                            { return std::isspace(c); }))
                throw std::invalid_argument(
                    "Truncated byte: " +
                    std::string(ptr, std::min(n, nCharsPerByte)));
            uint8_t value;
            std::from_chars_result conv =
                std::from_chars(ptr, ptr + nCharsPerByte, value, base);
            if (conv.ec != std::errc())
                throw std::system_error(std::make_error_code(conv.ec));
            if (conv.ptr != ptr + nCharsPerByte)
                throw std::invalid_argument(
                    "Invalid characters in byte: " +
                    std::string(ptr, ptr + nCharsPerByte));
            bytes.push_back(static_cast<std::byte>(value));
            n -= nCharsPerByte;
            ptr += nCharsPerByte;
        }
        return bytes;
    }

    std::string toStr(
        const std::byte *data, std::size_t n, std::size_t base,
        std::size_t nCharsPerByte)
    {
        if (n == 0)
            return "";
        std::string str((nCharsPerByte + 1) * n - 1, ' ');
        char *ptr = str.data();
        const uint8_t *data_ = reinterpret_cast<const uint8_t *>(data);
        while (n > 0)
        {

            std::to_chars_result result =
                std::to_chars(ptr, ptr + nCharsPerByte, *data_, base);
            if (result.ec != std::errc())
                throw std::system_error(std::make_error_code(result.ec));
            std::size_t offset = ptr + nCharsPerByte - result.ptr;
            if (offset)
            {
                std::memmove(ptr + offset, ptr, nCharsPerByte - offset);
                std::memset(ptr, '0', offset);
            }
            --n;
            ptr += nCharsPerByte + 1;
            data_ += 1;
        }
        return str;
    }
} // namespace

namespace Bytestream
{
    std::vector<std::byte> fromHex(const std::string &str)
    {
        return fromStr(str, 16, 2);
    }
    std::vector<std::byte> fromBinary(const std::string &str)
    {
        return fromStr(str, 2, 8);
    }

    std::string toHex(const void *data, std::size_t n)
    {
        return toStr(reinterpret_cast<const std::byte *>(data), n, 16, 2);
    }

    std::string toBinary(const void *data, std::size_t n)
    {
        return toStr(reinterpret_cast<const std::byte *>(data), n, 2, 8);
    }
} // namespace Bytestream