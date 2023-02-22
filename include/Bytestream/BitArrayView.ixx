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
    std::enable_if_t<!is_bitarray_v<T>, void> BitArrayView<E>::set(const T &value) {
        setFrom(ConstBitArrayView<E2>::viewOf(value));
    }

    template <Endian E>
    template <typename T>
    std::enable_if_t<!is_bitarray_v<T>, BitArrayView<E> &> BitArrayView<E>::operator=(
            const T &value) {
        set(value);
        return *this;
    }
} // namespace Bytestream