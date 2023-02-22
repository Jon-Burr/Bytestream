/**
 * @file ArrayForward.hxx
 * @brief Forward declarations for the array classes
 */

#ifndef BYTESTREAM_ARRAYFORWARD_HXX
#define BYTESTREAM_ARRAYFORWARD_HXX

#include "Bytestream/Endian.hxx"
#include <type_traits>

namespace Bytestream {
    template <Endian E = Endian::Native> class ConstBitArrayView;
    template <Endian E = Endian::Native> class BitArrayView;
    template <Endian E = Endian::Native> class BitArray;

    /// @brief Is the provided type a BitArray type
    /// @tparam T The type to check
    template <typename T>
    static constexpr inline bool is_bitarray_v =
            std::is_base_of_v<ConstBitArrayView<Endian::Big>, T> ||
            std::is_base_of_v<ConstBitArrayView<Endian::Little>, T>;
} // namespace Bytestream

#endif //> !BYTESTREAM_ARRAYFORWARD_HXX