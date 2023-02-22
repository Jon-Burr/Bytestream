/**
 * @file ArrayForward.hxx
 * @brief Forward declarations for the array classes
 */

#ifndef BYTESTREAM_ARRAYFORWARD_HXX
#define BYTESTREAM_ARRAYFORWARD_HXX

#include "Bytestream/Endian.hxx"

namespace Bytestream {
    template <Endian E = Endian::Native> class ConstBitArrayView;
    template <Endian E = Endian::Native> class BitArrayView;
    template <Endian E = Endian::Native> class BitArray;
} // namespace Bytestream

#endif //> !BYTESTREAM_ARRAYFORWARD_HXX