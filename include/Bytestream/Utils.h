/**
 * @file Utils.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Helpers functions for interpreting bytestreams
 * @version 0.1
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef BYTESTREAM_UTILS_H
#define BYTESTREAM_UTILS_H

#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Bytestream {
    /**
     * @brief Calculate the minimum number of bytes required to hold the given
     * number of bits
     */
    constexpr inline std::size_t nBytesToHold(std::size_t nBits) noexcept;

    /**
     * @brief Compile time check if a type is a unsigned integer type
     *
     * @tparam T The type to check
     */
    template <typename T> constexpr bool is_uint_v = std::is_integral_v<T> &&std::is_unsigned_v<T>;

    /**
     * @brief Helper struct to find the smallest native unsigned integer type
     * that can hold the specified number of bytes
     *
     * @tparam N The number of bytes the type must be able to hold
     */
    template <std::size_t N> struct BytesInfo {
        static_assert(N <= 8, "No native uint type larger than 8 bytes");
        using uint_t = std::conditional_t<
                N <= 1, uint8_t,
                std::conditional_t<
                        N <= 2, uint16_t, std::conditional_t<N <= 4, uint32_t, uint64_t>>>;
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

} // namespace Bytestream

#include "Bytestream/Utils.icc"
#endif //> !BYTESTREAM_TYPEUTILS_H