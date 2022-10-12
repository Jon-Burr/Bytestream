#ifndef BYTESTREAM_ENDIAN_H
#define BYTESTREAM_ENDIAN_H

// TODO: This makes this completely un platform indepenent for now...
namespace Bytestream
{
    enum class Endian
    {
        Little = __ORDER_LITTLE_ENDIAN__,
        Big = __ORDER_BIG_ENDIAN__,
        Native = __BYTE_ORDER__
    };
}

#endif //> !BYTESTREAM_ENDIAN_H