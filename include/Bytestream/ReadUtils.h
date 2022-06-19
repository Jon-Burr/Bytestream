/**
 * @file ReadUtils.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Helper functions to read information from byte arrays
 * @version 0.1
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 */

#ifndef BYTESTREAM_READUTILS_H
#define BYTESTREAM_READUTILS_H

#include "Bytestream/Utils.h"
#include <bit>
#include <cstddef>

namespace Bytestream
{
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
     * @brief Copy selected bits from the source array to the destination
     *
     * @param dest The destination array
     * @param src The source array
     * @param pos The position in the source array measured from the leftmost
     * bit
     * @param n The number of bits to copy
     *
     * The source array must be large enough to hold pos + n bits. The target
     * array must be large enough to hold n bits. The bits will be aligned to
     * the right of the target array. The two arrays must not overlap. Any
     * remaining bits to the left of the target array will be zeroed.
     */
    void copyBits(
        void *dest, const void *src, std::size_t pos, std::size_t n) noexcept;

    /**
     * @brief Get a pointer to the leftmost byte that will be used in the value
     *
     * @tparam T The type of value - must be an unsigned int
     * @param value The value
     * @param nBytes The number of bytes to be used in the value
     * @param zeroUnused If true, set unused bytes to 0
     * @return std::enable_if_t<is_uint_v<T>, std::byte *>
     */
    template <typename T>
    std::enable_if_t<is_uint_v<T>, std::byte *>
    ptrToStartByte(T &value, std::size_t nBytes, bool zeroUnused = true);

} // namespace Bytestream

#include "Bytestream/ReadUtils.icc"
#endif //> !BYTESTREAM_READUTILS_H