#include "Bytestream/ReadBytestreamHelpers.h"
#include "Bytestream/StringUtils.h"

namespace Bytestream {
    Reader::Reader(void *target, std::size_t nBits,
                   std::endian sourceEndianness)
            : m_target(reinterpret_cast<std::byte *>(target)), m_nBits(nBits),
              m_endianness(sourceEndianness) {}

    ReadBytestream &operator>>(ReadBytestream &stream, Reader reader) {
        stream.readBitsInto(reader.m_target, reader.m_nBits,
                            reader.m_endianness);
        return stream;
    }

    ExpectsHandle::ExpectsHandle(const void *expected, std::size_t nBits)
            : Reader(new std::byte[nBytesToHold(nBits)], nBits,
                     std::endian::native),
              m_expected(expected) {}

    ExpectsHandle::ExpectsHandle(const ConstByteArrayView &view)
            : ExpectsHandle(view.data(), view.size() * CHAR_BIT) {}

    ExpectsHandle::~ExpectsHandle() { delete[] m_target; }

    bool ExpectsHandle::good() const {
        return std::memcmp(m_expected, m_target, nBytesToHold(m_nBits));
    }

    void ExpectsHandle::throwIfBad() const {
        if (!good())
            throw std::invalid_argument(
                    "Excepted '" +
                    Bytestream::toHex(expected(), nBytesToHold(nBits())) +
                    "' '" +
                    Bytestream::toHex(received(), nBytesToHold(nBits())) + "'");
    }

    std::size_t ExpectsHandle::nBits() const { return m_nBits; }

    const void *ExpectsHandle::expected() const { return m_expected; }

    const void *ExpectsHandle::received() const { return m_target; }
} // namespace Bytestream