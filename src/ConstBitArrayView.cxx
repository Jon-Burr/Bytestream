#include "Bytestream/ConstBitArrayView.hxx"
#include "Bytestream/BitArray.hxx"
#include "Bytestream/string.hxx"

#include <cassert>
#include <charconv>
#include <memory>
#include <stdexcept>

namespace Bytestream {

    template <Endian E>
    ConstBitArrayView<E>::ConstBitArrayView(const void *data, std::size_t nBits, uint8_t offset)
            : m_data(static_cast<const std::byte *>(data)), m_nBits(nBits), m_offset(offset) {
        assert(offset < CHAR_BIT);
    }

    template <Endian E> bool ConstBitArrayView<E>::operator[](std::size_t idx) const {
        std::size_t offset = m_offset + idx;
        std::size_t byte = offset / CHAR_BIT;
        uint8_t bit = offset % CHAR_BIT;
        std::byte mask = std::byte(1) << bit;
        return bool(*(byte_begin() + byte) & mask);
    }

    template <Endian E> bool ConstBitArrayView<E>::at(std::size_t idx) const {
        if (idx >= size())
            throw std::out_of_range(std::to_string(idx));
        return this->operator[](idx);
    }

    template <Endian E>
    ConstBitArrayView<E> ConstBitArrayView<E>::sub(std::size_t pos, std::size_t nBits) const {
        if (pos >= m_nBits)
            throw std::out_of_range(std::to_string(pos));
        pos += m_offset;
        std::size_t byte = pos / CHAR_BIT;
        std::size_t bit = pos % CHAR_BIT;
        nBits = std::min(nBits, m_nBits - pos);
        if constexpr (E == Endian::Little)
            return ConstBitArrayView(lsb() + byte, nBits, bit);
        else
            return ConstBitArrayView(msb() - byte, nBits, bit);
    }

    template <Endian E> BitArray<E> ConstBitArrayView<E>::copy() const {
        return BitArray<E>(*this);
    }

    template <Endian E> BitArray<swapped_v<E>> ConstBitArrayView<E>::swapEndianness() const {
        return BitArray<swapped_v<E>>(*this);
    }

    template <Endian E> std::string ConstBitArrayView<E>::toBinary() const {
        std::string result;
        std::size_t nBytes = nCoveredBytes();
        result.reserve(CHAR_BIT * nBytes);
        std::size_t leftOffset = (CHAR_BIT * nBytes) - (m_nBits + m_offset);
        // Start from the most-significant byte as that is leftmost in the string
        for (auto itr = byte_rbegin(); itr != byte_rend(); ++itr)
            result += Bytestream::toBinary(*itr);
        return result.substr(leftOffset, m_nBits);
    }

    template <Endian E> const std::byte *ConstBitArrayView<E>::lsb() const {
        if constexpr (E == Endian::Little)
            return m_data;
        else
            return m_data + nCoveredBytes() - 1;
    }

    template <Endian E> const std::byte *ConstBitArrayView<E>::msb() const {
        if constexpr (E == Endian::Little)
            return m_data + nCoveredBytes() - 1;
        else
            return m_data;
    }

    template <Endian E> ConstByteIterator<E> ConstBitArrayView<E>::byte_begin() const {
        if constexpr (E == Endian::Little)
            return lsb();
        else
            return std::make_reverse_iterator(lsb()) - 1;
    }

    template <Endian E> ConstByteIterator<E> ConstBitArrayView<E>::byte_end() const {
        return byte_begin() + nCoveredBytes();
    }

    template <Endian E> ReverseConstByteIterator<E> ConstBitArrayView<E>::byte_rbegin() const {
        if constexpr (E == Endian::Little)
            return std::make_reverse_iterator(msb()) - 1;
        else
            return msb();
    }

    template <Endian E> ReverseConstByteIterator<E> ConstBitArrayView<E>::byte_rend() const {
        return byte_rbegin() + nCoveredBytes();
    }

    template <Endian E>
    AlignedByteIterator<ConstByteIterator<E>> ConstBitArrayView<E>::aligned_byte_begin() const {
        return {byte_begin(), this->offset(), this->size()};
    }

    template <Endian E>
    AlignedByteIterator<ConstByteIterator<E>> ConstBitArrayView<E>::aligned_byte_end() const {
        return {byte_begin() + nBytesToHold(this->size()), this->offset(), 0};
    }

    template <Endian E> uint8_t ConstBitArrayView<E>::msbPad() const {
        uint8_t offset = (m_offset + m_nBits) / CHAR_BIT;
        return offset == 0 ? 0 : CHAR_BIT - offset;
    }

    template class ConstBitArrayView<Endian::Little>;
    template class ConstBitArrayView<Endian::Big>;
} // namespace Bytestream
