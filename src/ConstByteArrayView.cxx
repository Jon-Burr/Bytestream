#include "Bytestream/ConstByteArrayView.h"
#include "Bytestream/ReadUtils.h"
#include "Bytestream/StringUtils.h"

#include <cstring>
#include <stdexcept>
#include <string>

namespace Bytestream {
    ConstByteArrayView::ConstByteArrayView(const void *data, std::size_t nBytes)
            : m_data(reinterpret_cast<const std::byte *>(data)),
              m_nBytes(nBytes) {}

    std::byte ConstByteArrayView::at(std::size_t idx) const {
        if (idx >= m_nBytes)
            throw std::out_of_range(std::to_string(idx));
        return this->operator[](idx);
    }

    std::string ConstByteArrayView::toHex() const {
        return Bytestream::toHex(data(), size());
    }

    std::string ConstByteArrayView::toBinary() const {
        return Bytestream::toBinary(data(), size());
    }

    ConstByteArrayView ConstByteArrayView::view(std::size_t idx,
                                                std::size_t nBytes) const {
        if (idx + nBytes > size())
            throw std::out_of_range(std::to_string(idx + nBytes));
        return ConstByteArrayView(m_data + size() - idx - nBytes, nBytes);
    }

    void ConstByteArrayView::readBitsInto(void *target, std::size_t bitPos,
                                          std::size_t nBits,
                                          Endian sourceEndianness) const {
        copyBits(target, data(), size() * CHAR_BIT - bitPos - nBits, nBits);
        if (sourceEndianness != Endian::Native)
            swapEndiannessInPlace(target, nBytesToHold(nBits));
    }

    void ConstByteArrayView::readBytesInto(void *target, std::size_t bytePos,
                                           std::size_t nBytes,
                                           Endian sourceEndianness) const {
        std::memcpy(target, data() + size() - bytePos - nBytes, nBytes);
        if (sourceEndianness != Endian::Native)
            swapEndiannessInPlace(target, nBytes);
    }

    bool operator==(const ConstByteArrayView &lhs,
                    const ConstByteArrayView &rhs) {
        return lhs.size() == rhs.size() &&
               (std::memcmp(lhs.data(), rhs.data(), lhs.size()) == 0);
    }

    bool operator!=(const ConstByteArrayView &lhs,
                    const ConstByteArrayView &rhs) {
        return !(lhs == rhs);
    }

    std::ostream &operator<<(std::ostream &os, const ConstByteArrayView &view) {
        if (os.flags() & os.hex)
            return os << view.toHex();
        else
            return os << view.toBinary();
    }
} // namespace Bytestream