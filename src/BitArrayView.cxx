#include "Bytestream/BitArrayView.hxx"

#include <stdexcept>

namespace Bytestream {

    template <Endian E>
    BitArrayView<E>::BitArrayView(void *data, std::size_t nBits, uint8_t offset)
            : ConstBitArrayView<E>(data, nBits, offset) {}

    template <Endian E>
    template <Endian E2>
    BitArrayView<E> &BitArrayView<E>::operator=(const ConstBitArrayView<E2> &other) {
        auto itr = this->aligned_byte_begin();
        auto end = this->aligned_byte_end();
        auto otherItr = other.aligned_byte_begin();
        auto otherEnd = other.aligned_byte_end();
        for (; itr != end && otherItr != otherEnd; ++itr, ++otherItr)
            *itr = *otherItr;
        // Zero any remaining bytes
        for (; itr != end; ++itr)
            *itr = std::byte(0);
        return *this;
    }

    template <Endian E> BitProxy BitArrayView<E>::operator[](std::size_t idx) {
        std::size_t offset = this->m_offset + idx;
        return BitProxy(&*(byte_begin() + offset / CHAR_BIT), offset % CHAR_BIT);
    }

    template <Endian E> BitProxy BitArrayView<E>::at(std::size_t idx) {
        if (idx >= this->size())
            throw std::out_of_range(std::to_string(idx));
        return this->operator[](idx);
    }

    template <Endian E> BitArrayView<E> BitArrayView<E>::sub(std::size_t pos, std::size_t nBits) {
        if (pos >= this->m_nBits)
            throw std::out_of_range(std::to_string(pos));
        pos += this->m_offset;
        std::size_t byte = pos / CHAR_BIT;
        std::size_t bit = pos % CHAR_BIT;
        nBits = std::min(nBits, this->m_nBits - pos);
        if constexpr (E == Endian::Little)
            return BitArrayView(lsb() + byte, nBits, bit);
        else
            return BitArrayView(msb() - byte, nBits, bit);
    }

    template <Endian E> ByteIterator<E> BitArrayView<E>::byte_begin() {
        if constexpr (E == Endian::Little)
            return lsb();
        else
            return std::make_reverse_iterator(lsb()) - 1;
    }

    template <Endian E> ByteIterator<E> BitArrayView<E>::byte_end() {
        return byte_begin() + this->nCoveredBytes();
    }

    template <Endian E> ReverseByteIterator<E> BitArrayView<E>::byte_rbegin() {
        if constexpr (E == Endian::Little)
            return std::make_reverse_iterator(msb()) - 1;
        else
            return msb();
    }

    template <Endian E> ReverseByteIterator<E> BitArrayView<E>::byte_rend() {
        return byte_rbegin() + this->nCoveredBytes();
    }

    template <Endian E> AlignedByteIterator<ByteIterator<E>> BitArrayView<E>::aligned_byte_begin() {
        return {byte_begin(), this->offset(), this->size()};
    }

    template <Endian E> AlignedByteIterator<ByteIterator<E>> BitArrayView<E>::aligned_byte_end() {
        return {byte_begin() + nBytesToHold(this->size()), this->offset(), 0};
    }

    template class BitArrayView<Endian::Little>;
    template class BitArrayView<Endian::Big>;
#ifndef NODOXYGEN
    // Instaniate the setting operator
    template BitArrayView<Endian::Little> &BitArrayView<Endian::Little>::operator=
            <Endian::Little>(const ConstBitArrayView<Endian::Little> &);
    template BitArrayView<Endian::Little> &BitArrayView<Endian::Little>::operator=
            <Endian::Big>(const ConstBitArrayView<Endian::Big> &);
    template BitArrayView<Endian::Big> &BitArrayView<Endian::Big>::operator=
            <Endian::Little>(const ConstBitArrayView<Endian::Little> &);
    template BitArrayView<Endian::Big> &BitArrayView<Endian::Big>::operator=
            <Endian::Big>(const ConstBitArrayView<Endian::Big> &);
#endif
} // namespace Bytestream