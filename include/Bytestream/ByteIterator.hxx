/// @file ByteIterator.hxx
/// @brief Helper type defs for byte iterators

#ifndef BYTESTREAM_BYTEITERATOR_HXX
#define BYTESTREAM_BYTEITERATOR_HXX

#include "Bytestream/Endian.hxx"

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace Bytestream {
    /// @brief Mutable byte iterator from LSB to MSB
    /// @tparam E The endianness of the underlying memory
    template <Endian E>
    using ByteIterator = std::conditional_t<
            E == Endian::Little, std::byte *, std::reverse_iterator<std::byte *>>;
    /// @brief Mutable byte iterator from MSB to LSB
    /// @tparam E The endianness of the underlying memory
    template <Endian E>
    using ReverseByteIterator = std::conditional_t<
            E == Endian::Little, std::reverse_iterator<std::byte *>, std::byte *>;
    /// @brief Const byte iterator from MSB to LSB
    /// @tparam E The endianness of the underlying memory
    template <Endian E>
    using ConstByteIterator = std::conditional_t<
            E == Endian::Little, const std::byte *, std::reverse_iterator<const std::byte *>>;
    /// @brief Const byte iterator from LSB to MSB
    /// @tparam E The endianness of the underlying memory
    template <Endian E>
    using ReverseConstByteIterator = std::conditional_t<
            E == Endian::Little, std::reverse_iterator<const std::byte *>, const std::byte *>;
} // namespace Bytestream

#endif //> !BYTESTREAM_BYTEITERATOR_HXX