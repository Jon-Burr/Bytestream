#include "Bytestream/ByteArrayView.h"

#include <cstring>

namespace Bytestream {
    ByteArrayView::ByteArrayView(void *data, std::size_t nBytes)
            : ConstByteArrayView(data, nBytes) {}

    void ByteArrayView::copyFrom(const ConstByteArrayView &other) {
        copyFrom(other.data(), other.size());
    }

    void ByteArrayView::copyFrom(const void *src, std::size_t nBytes) {
        if (nBytes >= size()) {
            std::memcpy(data(), reinterpret_cast<const std::byte *>(src) + nBytes - size(), size());
        } else {
            std::memcpy(data() + size() - nBytes, src, nBytes);
        }
    }

    std::byte &ByteArrayView::at(std::size_t idx) {
        if (idx >= size())
            throw std::out_of_range(std::to_string(idx));
        return this->operator[](idx);
    }

    ByteArrayView ByteArrayView::view(std::size_t idx, std::size_t nBytes) {
        if (idx + nBytes > size())
            throw std::out_of_range(std::to_string(idx + nBytes));
        return ByteArrayView(data() + size() - idx - nBytes, nBytes);
    }
} // namespace Bytestream