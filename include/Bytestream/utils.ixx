#include <climits>

namespace Bytestream {
    constexpr std::size_t nBytesToHold(std::size_t nBits) noexcept {
        return nBits / CHAR_BIT + (nBits % CHAR_BIT != 0);
    }

    constexpr std::byte reverseBits(std::byte value) noexcept {
        return (((value & std::byte{0b00000001}) << 7) | ((value & std::byte{0b00000010}) << 5) |
                ((value & std::byte{0b00000100}) << 3) | ((value & std::byte{0b00001000}) << 1) |
                ((value & std::byte{0b00010000}) >> 1) | ((value & std::byte{0b00100000}) >> 3) |
                ((value & std::byte{0b01000000}) >> 5) | ((value & std::byte{0b10000000}) >> 7));
    }

    constexpr uint8_t swapEndianness(uint8_t value) noexcept { return value; }

    constexpr uint16_t swapEndianness(uint16_t value) noexcept {
        return ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
    }

    constexpr uint32_t swapEndianness(uint32_t value) noexcept {
        return (((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) |
                ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24));
    }

    constexpr uint64_t swapEndianness(uint64_t value) noexcept {
        return (((value & 0xFF00000000000000ull) >> 56) | ((value & 0x00FF000000000000ull) >> 40) |
                ((value & 0x0000FF0000000000ull) >> 24) | ((value & 0x000000FF00000000ull) >> 8) |
                ((value & 0x00000000FF000000ull) << 8) | ((value & 0x0000000000FF0000ull) << 24) |
                ((value & 0x000000000000FF00ull) << 40) | ((value & 0x00000000000000FFull) << 56));
    }

    constexpr inline std::byte createLeftMask(std::size_t nBits) noexcept {
        return ~(~std::byte{0} >> nBits);
    }

    constexpr inline std::byte createRightMask(std::size_t nBits) noexcept {
        return ~(~std::byte{0} << nBits);
    }

    constexpr inline void copyPartialByte(std::byte &dest, std::byte src, std::byte mask) {
        // First zero the bits
        dest &= ~mask;
        // Now set the correct bits in the target
        dest |= (mask & src);
    }

} // namespace Bytestream