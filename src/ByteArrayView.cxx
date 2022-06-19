#include "Bytestream/ByteArrayView.h"

namespace Bytestream
{
    ByteArrayView::ByteArrayView(void *data, std::size_t nBytes)
        : ConstByteArrayView(data, nBytes)
    {
    }

    std::byte &ByteArrayView::at(std::size_t idx)
    {
        if (idx >= size())
            throw std::out_of_range(std::to_string(idx));
        return this->operator[](idx);
    }

    ByteArrayView ByteArrayView::view(std::size_t idx, std::size_t nBytes)
    {
        if (idx + nBytes > size())
            throw std::out_of_range(std::to_string(idx + nBytes));
        return ByteArrayView(data() + size() - idx - nBytes - 1, nBytes);
    }
} // namespace Bytestream