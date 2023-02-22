namespace Bytestream {
#ifndef NODOXYGEN
    template <Endian E>
    template <typename T>
    ConstBitArrayView<E> ConstBitArrayView<E>::viewOf(
            const T &value, std::size_t pos, std::size_t nBits) {
        return ConstBitArrayView::viewOf(value).sub(pos, nBits);
    }
    template <Endian E>
    template <typename T>
    ConstBitArrayView<E> ConstBitArrayView<E>::viewOf(const T &value) {
        return ConstBitArrayView(&value, sizeof(T) * CHAR_BIT, 0);
    }
#endif

    template <Endian E>
    template <typename T>
    std::enable_if_t<std::is_default_constructible_v<T>, T> ConstBitArrayView<E>::asType() const {
        T value{0};
        std::byte *ptr = reinterpret_cast<std::byte *>(&value);
        std::byte *valueEnd = ptr + sizeof(T);
        auto itr = aligned_byte_begin();
        auto end = aligned_byte_end();
        for (; itr != end && ptr != valueEnd; ++itr, ++ptr)
            *ptr = *itr;
        return value;
    }
} // namespace Bytestream