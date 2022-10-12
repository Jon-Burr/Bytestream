#include "Bytestream/ByteArrayView.h"

#include <cstring>

namespace Bytestream
{
    ByteArrayView::ByteArrayView(void *data, std::size_t nBytes)
        : ConstByteArrayView(data, nBytes) {}

    void ByteArrayView::copyFrom(const ConstByteArrayView &other)
    {
        if (other.size() >= size())
        {
            std::memcpy(data(), other.data() + other.size() - size(), size());
        }
        else
        {
            std::memcpy(data() + size() - other.size(), other.data(), size());
        }
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
        return ByteArrayView(data() + size() - idx - nBytes, nBytes);
    }
} // namespace Bytestream