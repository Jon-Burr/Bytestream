#include "Bytestream/ReadUtils.h"
#include "Bytestream/Utils.h"

#include <cassert>
#include <cstring>

#include <iostream>

namespace Bytestream
{
    void copyBits(
        void *dest, const void *src, std::size_t pos, std::size_t n) noexcept
    {
        if (n == 0)
            return;
        const std::byte *src_ = reinterpret_cast<const std::byte *>(src);
        std::byte *dest_ = reinterpret_cast<std::byte *>(dest);
        std::size_t nBitsFromLeft = pos + n;
        std::size_t leftShift = nBitsFromLeft % CHAR_BIT;
        std::size_t sourceRightByte = nBytesToHold(nBitsFromLeft);
        std::size_t destSize = nBytesToHold(n);
        std::size_t startWholeByte = sourceRightByte - destSize;
        std::byte mask =
            n % CHAR_BIT ? createRightMask(n % CHAR_BIT) : std::byte{0xFF};
        if (leftShift == 0) {
            // We can just copy whole bytes and then mask out the leftmost bits
            std::memcpy(dest_, src_ + startWholeByte, destSize);
        } else {
            std::size_t startWholeByte = sourceRightByte - destSize;
            *dest_ = *(src_ + startWholeByte) >> (CHAR_BIT - leftShift);
            if (startWholeByte > 0)
                *dest_ |= *(src_ + startWholeByte - 1) << leftShift;
            for (std::size_t idx = 1; idx < destSize; ++idx)
                *(dest_ + idx) =
                    *(src_ + startWholeByte + idx - 1) << leftShift |
                    *(src_ + startWholeByte + idx) >> (CHAR_BIT - leftShift);
        }
        *dest_ &= mask;
    }
} // namespace Bytestream
