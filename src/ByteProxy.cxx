#include "Bytestream/ByteProxy.hxx"
#include "Bytestream/utils.hxx"

#include <iterator>

namespace Bytestream {

    template <typename Iterator>
    ConstByteProxy<Iterator>::ConstByteProxy(Iterator itr, uint8_t offset, uint8_t size)
            : m_itr(itr), m_offset(offset), m_size(size) {}

    template <typename Iterator> std::byte ConstByteProxy<Iterator>::get() const {
        std::byte value = *m_itr >> m_offset;
        if (m_offset + m_size > CHAR_BIT)
            value |= *(m_itr + 1) << (CHAR_BIT - m_offset);
        return m_size >= CHAR_BIT ? value : value & createRightMask(m_size);
    }
    
    template <typename Iterator> void ByteProxy<Iterator>::set(std::byte b) {
        copyPartialByte(
                *(this->m_itr), b << this->m_offset,
                createRightMask(this->m_size) << this->m_offset);
        if (this->m_offset + this->m_size > CHAR_BIT)
            copyPartialByte(
                    *(this->m_itr + 1), b >> (CHAR_BIT - this->m_offset),
                    createRightMask(this->m_size) >> (CHAR_BIT - this->m_offset));
    }

    template class ConstByteProxy<std::byte *>;
    template class ConstByteProxy<std::reverse_iterator<std::byte *>>;
    template class ConstByteProxy<const std::byte *>;
    template class ConstByteProxy<std::reverse_iterator<const std::byte *>>;

    template class ByteProxy<std::byte *>;
    template class ByteProxy<std::reverse_iterator<std::byte *>>;

} // namespace Bytestream