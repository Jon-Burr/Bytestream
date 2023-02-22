/**
 * @file Endian.hxx
 * @brief Provide the endianness enum outside C++20
 */

#ifndef BYTESTREAM_ENDIAN_HXX
#define BYTESTREAM_ENDIAN_HXX

#include <type_traits>

#if __cplusplus >= 202002L
#include <bit>
namespace Bytestream {
    enum class Endian {
        Little = std::endian::little,
        Big = std::endian::big,
        Native = std::endian::native
    };
} // namespace Bytestream
#else
namespace Bytestream {
    //  TODO this is not  platform independent
    enum class Endian {
        Little = __ORDER_LITTLE_ENDIAN__,
        Big = __ORDER_BIG_ENDIAN__,
        Native = __BYTE_ORDER__
    };
} // namespace Bytestream
#endif //> !C++ 20

namespace Bytestream {
#ifndef NODOXYGEN
    namespace detail {
        template <Endian E> struct swapped_t;
        template <> struct swapped_t<Endian::Little> {
            static constexpr inline Endian value = Endian::Big;
        };
        template <> struct swapped_t<Endian::Big> {
            static constexpr inline Endian value = Endian::Little;
        };
    }  // namespace detail
#endif //> !NODOXYGEN
    /// @brief Get the swapped endianness
    /// @tparam E the endianness to swap
    template <Endian E> static constexpr inline Endian swapped_v = detail::swapped_t<E>::value;
} // namespace Bytestream

#endif //> !BYTESTREAM_ENDIAN_HXX