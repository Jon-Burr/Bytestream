/// @file utils.hxx
/// @brief Helper functions for interpreting bytes

#ifndef BYTESTREAM_UTILS_HXX
#define BYTESTREAM_UTILS_HXX

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Bytestream {
    /// @brief The number of bytes needed to hold the specified number of bits
    constexpr inline std::size_t nBytesToHold(std::size_t nBits) noexcept;

    /**
     * @brief Compile time check if a type is a unsigned integer type
     *
     * @tparam T The type to check
     */
    template <typename T> constexpr bool is_uint_v = std::is_integral_v<T> && std::is_unsigned_v<T>;

    /**
     * @brief Helper struct to find the smallest native unsigned integer type
     * that can hold the specified number of bytes
     *
     * @tparam N The number of bytes the type must be able to hold
     */
    template <std::size_t N> struct BytesInfo {
        static_assert(N <= 8, "No native uint type larger than 8 bytes");
        /// @brief The smallest unsigned integer type that contains thiis number of bytes
        using uint_t = std::conditional_t<
                N <= 1, uint8_t,
                std::conditional_t<
                        N <= 2, uint16_t, std::conditional_t<N <= 4, uint32_t, uint64_t>>>;
        /// @brief The number of bytes held by this value
        inline static constexpr std::size_t nBytes = sizeof(uint_t);
    };

    /**
     * @brief Helper struct to find the smallest native unsigned integer type
     * that can hold the specified number of bits
     *
     * @tparam N the number of bits the type must be able to hold
     */
    template <std::size_t N> using BitsInfo = BytesInfo<nBytesToHold(N)>;

    /// Reverse the bits in the provided byte
    constexpr inline std::byte reverseBits(std::byte value) noexcept;
    /// Swap the endianness of a number
    constexpr inline uint8_t swapEndianness(uint8_t value) noexcept;
    /// Swap the endianness of a number
    constexpr inline uint16_t swapEndianness(uint16_t value) noexcept;
    /// Swap the endianness of a number
    constexpr inline uint32_t swapEndianness(uint32_t value) noexcept;
    /// Swap the endianness of a number
    constexpr inline uint64_t swapEndianness(uint64_t value) noexcept;
    /// Swap the endianness of a byte array in place
    void swapEndiannessInPlace(void *data, std::size_t size);

    /**
     * @brief Create a bitmask that keeps the n leftmost bits
     *
     * If nBits is larger than 8 all bits are kept
     */
    constexpr inline std::byte createLeftMask(std::size_t nBits) noexcept;

    /**
     * @brief Create a bitmask that keeps the n rightmost bits
     *
     * If nBits is larger than 8 all bits are kept
     */
    constexpr inline std::byte createRightMask(std::size_t nBits) noexcept;

    /**
     * @brief Copy part of a byte from src to dest
     * @param dest The target to be changed
     * @param src The source data
     * @param mask Only the bits which are 1 here will be copied
     */
    constexpr inline void copyPartialByte(std::byte &dest, std::byte src, std::byte mask);
} // namespace Bytestream

#include "Bytestream/utils.ixx"
#endif //> !BYTESTREAM_UTILS_HXX