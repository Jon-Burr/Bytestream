#include "Bytestream/ReadBytestream.h"

namespace Bytestream
{
    ReadBytestream::ReadBytestream(const ConstByteArrayView &view)
        : m_view(view)
    {
    }

    std::size_t ReadBytestream::position() const { return m_pos; }

    std::size_t ReadBytestream::size() const
    {
        return m_view.size() * CHAR_BIT;
    }

    std::size_t ReadBytestream::remaining() const
    {
        return size() - position();
    }

    bool ReadBytestream::exhausted() const { return remaining() == 0; }

    void ReadBytestream::advance(std::size_t n)
    {
        if (n > remaining())
            throw std::out_of_range(std::to_string(position() + n));
        m_pos += n;
    }

    void ReadBytestream::rewind(std::size_t n)
    {
        if (n > position())
            throw std::out_of_range("-" + std::to_string(n - position()));
        m_pos -= n;
    }

    void ReadBytestream::seek(std::size_t pos)
    {
        if (pos >= size())
            throw std::out_of_range(std::to_string(pos));
        m_pos = pos;
    }

    void ReadBytestream::reset() { m_pos = 0; }

    bool ReadBytestream::onByteBoundary() const
    {
        return m_pos % CHAR_BIT == 0;
    }

    void ReadBytestream::readBitsInto(std::byte *target, std::size_t nBits)
    {
        m_view.readBitsInto(target, m_pos, nBits);
        advance(nBits);
    }

    ConstByteArrayView ReadBytestream::readUnformattedBytes(std::size_t nBytes)
    {
        ConstByteArrayView view = m_view.view(m_pos * CHAR_BIT, nBytes);
        advance(nBytes * CHAR_BIT);
        return view;
    }

} // namespace Bytestream