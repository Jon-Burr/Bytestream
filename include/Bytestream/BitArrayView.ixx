namespace Bytestream {
#ifndef NODOXYGEN
    template <Endian E>
    template <typename T>
    BitArrayView<E> BitArrayView<E>::viewOf(T &value, std::size_t pos, std::size_t nBits) {
        return BitArrayView::viewOf(value).sub(pos, nBits);
    }

    template <Endian E> template <typename T> BitArrayView<E> BitArrayView<E>::viewOf(T &value) {
        return BitArrayView(&value, sizeof(T) * CHAR_BIT, 0);
    }
#endif
    template <Endian E>
    template <typename T, Endian E2>
    void BitArrayView<E>::setFrom(const T &value) {
        *this = ConstBitArrayView<E2>::viewOf(value);
    }
} // namespace Bytestream