#include "Bytestream/ReadBytestream.h"
#include "Bytestream/ByteArray.h"

namespace Bytestream {
    ReadBytestream::ReadBytestream(const ConstByteArrayView &view) : m_view(view) {}

    std::size_t ReadBytestream::position() const { return m_pos; }

    std::size_t ReadBytestream::size() const { return m_view.size() * CHAR_BIT; }

    std::size_t ReadBytestream::remaining() const { return size() - position(); }

    bool ReadBytestream::exhausted() const { return remaining() == 0; }

    void ReadBytestream::advance(std::size_t n) {
        if (!good())
            return;
        if (n > remaining()) {
            m_pos = size();
            seteof();
            return;
        }
        m_pos += n;
    }

    void ReadBytestream::rewind(std::size_t n) {
        if (!good())
            return;
        if (n > position()) {
            m_pos = size();
            seteof();
            return;
        }
        m_pos -= n;
    }

    void ReadBytestream::seek(std::size_t pos) {
        if (!good())
            return;
        if (pos >= size()) {
            m_pos = size();
            seteof();
            return;
        }
        m_pos = pos;
    }

    void ReadBytestream::reset() {
        clear();
        m_pos = 0;
    }

    bool ReadBytestream::onByteBoundary() const { return m_pos % CHAR_BIT == 0; }

    void ReadBytestream::readBitsInto(
            std::byte *target, std::size_t nBits, Endian sourceEndianness) {
        if (!good())
            return;
        if (nBits > remaining()) {
            setstate(std::ios_base::failbit | std::ios_base::eofbit);
            return;
        }
        m_view.readBitsInto(target, m_pos, nBits, sourceEndianness);
        m_pos += nBits;
    }

    ConstByteArrayView ReadBytestream::readUnformattedBytes(std::size_t nBytes) {
        if (!good())
            return ConstByteArrayView(nullptr, 0);
        std::size_t nBits = nBytes * CHAR_BIT;
        if (nBits > remaining()) {
            setstate(std::ios_base::failbit | std::ios_base::eofbit);
            return ConstByteArrayView(nullptr, 0);
        }
        ConstByteArrayView view = m_view.view(m_pos * CHAR_BIT, nBytes);
        m_pos += nBits;
        return view;
    }

} // namespace Bytestream