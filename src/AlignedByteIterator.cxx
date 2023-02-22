#include "Bytestream/AlignedByteIterator.hxx"

#include <algorithm>

namespace Bytestream {

    template <typename Iterator>
    AlignedByteIterator<Iterator>::AlignedByteIterator(
            Iterator itr, uint8_t offset, std::size_t size)
            : m_itr(itr), m_offset(offset), m_size(size) {}

    template <typename Iterator>
    bool AlignedByteIterator<Iterator>::operator==(
            const AlignedByteIterator<Iterator> &other) const {
        return m_itr == other.m_itr && m_offset == other.m_offset && m_size == other.m_size;
    }

    template <typename Iterator>
    bool AlignedByteIterator<Iterator>::operator!=(
            const AlignedByteIterator<Iterator> &other) const {
        return !(*this == other);
    }

    template <typename Iterator>
    typename AlignedByteIterator<Iterator>::reference AlignedByteIterator<Iterator>::operator*()
            const {
        return makeByteProxy(m_itr, m_offset, m_size);
    }

    template <typename Iterator>
    AlignedByteIterator<Iterator> &AlignedByteIterator<Iterator>::operator++() {
        ++m_itr;
        if (m_size < CHAR_BIT)
            m_size = 0;
        else
            m_size -= CHAR_BIT;
        return *this;
    }

    template <typename Iterator>
    AlignedByteIterator<Iterator> AlignedByteIterator<Iterator>::operator++(int) {
        AlignedByteIterator<Iterator> copy(*this);
        this->operator++();
        return copy;
    }

    template class AlignedByteIterator<std::byte *>;
    template class AlignedByteIterator<std::reverse_iterator<std::byte *>>;
    template class AlignedByteIterator<const std::byte *>;
    template class AlignedByteIterator<std::reverse_iterator<const std::byte *>>;
} // namespace Bytestream