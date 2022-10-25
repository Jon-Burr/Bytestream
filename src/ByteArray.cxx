#include "Bytestream/ByteArray.h"
#include "Bytestream/StringUtils.h"

namespace Bytestream {
    ByteArray::ByteArray() : ByteArrayView(nullptr, 0) {}

    ByteArray::ByteArray(std::size_t n) : ByteArrayView(new std::byte[n], n) {}

    ByteArray::ByteArray(std::size_t n, std::byte value) : ByteArray(n) {
        std::memset(data(), std::to_integer<uint8_t>(value), n);
    }

    ByteArray::ByteArray(const void *src, std::size_t n) : ByteArray(n) {
        std::memcpy(data(), src, n);
    }

    ByteArray::ByteArray(const ConstByteArrayView &other) : ByteArray(other.data(), other.size()) {}

    ByteArray::ByteArray(ByteArray &&other) : ByteArrayView(other.data(), other.size()) {
        other.m_owning = false;
    }

    ByteArray::~ByteArray() {
        if (m_owning)
            delete[](data());
    }

    ByteArray ByteArray::fromHex(const std::string &hex) {
        std::vector<std::byte> data = Bytestream::fromHex(hex);
        return ByteArray(data.data(), data.size());
    }

    ByteArray ByteArray::fromBinary(const std::string &binary) {
        std::vector<std::byte> data = Bytestream::fromBinary(binary);
        return ByteArray(data.data(), data.size());
    }
} // namespace Bytestream