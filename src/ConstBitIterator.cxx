#include "Bytestream/ConstBitIterator.hxx"

#include <climits>

namespace Bytestream {

    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD>::ConstBitIterator(byte_itr_t itr, uint8_t offset)
            : m_itr(itr), m_offset(offset) {}

    template <Endian E, bool FORWARD>
    typename ConstBitIterator<E, FORWARD>::reference ConstBitIterator<E, FORWARD>::operator*()
            const {
        return bool((*m_itr) & std::byte(1) << m_offset);
    }

    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> &ConstBitIterator<E, FORWARD>::operator++() {
        if constexpr (FORWARD) {
            if (++m_offset == CHAR_BIT) {
                m_offset = 0;
                ++m_itr;
            }
        } else {
            if (m_offset == 0) {
                m_offset = 7;
                --m_itr;
            } else
                --m_offset;
        }
        return *this;
    }

    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> ConstBitIterator<E, FORWARD>::operator++(int) {
        ConstBitIterator tmp = *this;
        this->operator++();
        return tmp;
    }

    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> &ConstBitIterator<E, FORWARD>::operator+=(difference_type step) {
        if constexpr (!FORWARD)
            step = -step;
        step += m_offset;
        std::ptrdiff_t byte = step / CHAR_BIT;
        std::ptrdiff_t bit = step % CHAR_BIT;
        if (bit < 0) {
            --byte;
            bit += CHAR_BIT;
        }
        m_itr += byte;
        m_offset = bit;
        return *this;
    }

    template <Endian E, bool FORWARD>
    ConstBitIterator<E, FORWARD> &ConstBitIterator<E, FORWARD>::operator-=(difference_type step) {
        return operator+=(-step);
    }

    template class ConstBitIterator<Endian::Little, true>;
    template class ConstBitIterator<Endian::Little, false>;
    template class ConstBitIterator<Endian::Big, true>;
    template class ConstBitIterator<Endian::Big, false>;
} // namespace Bytestream
