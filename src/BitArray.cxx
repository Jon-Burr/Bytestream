#include "Bytestream/BitArray.hxx"
#include "Bytestream/AlignedByteIterator.hxx"
#include "Bytestream/utils.hxx"

#include <algorithm>
#include <charconv>
#include <cstring>
#include <stdexcept>
#include <system_error>

namespace {
    using namespace Bytestream;
    template <Endian E>
    BitArray<E> fromStr(
            const std::string &data_, const std::size_t base, const std::size_t bitsPerChar) {
        // Start by removing any spaces
        std::string data;
        data.reserve(data_.size());
        std::copy_if(data_.begin(), data_.end(), std::back_inserter(data), [](unsigned char c) {
            return !std::isspace(c);
        });
        BitArray<E> array(data.size() * bitsPerChar, 0);
        // Go byte by byte in the underlying array. The string starts with the MSB so so will we
        const char *ptr = data.data();
        const char *strEnd = data.data() + data.size();
        const std::size_t step = CHAR_BIT / bitsPerChar;
        for (auto itr = array.byte_rbegin(); itr != array.byte_rend(); ++itr) {
            uint8_t value;
            const char *end = std::min(ptr + step, strEnd);
            std::from_chars_result res = std::from_chars(ptr, end, value, base);
            if (res.ec != std::errc())
                throw std::system_error(std::make_error_code(res.ec));
            if (res.ptr != end)
                throw std::invalid_argument(
                        "Invalid characters in  byte: " + std::string(ptr, end));
            *itr = std::byte{value};
            ptr = res.ptr;
        }
        return array;
    }
} // namespace

namespace Bytestream {

    template <Endian E> BitArray<E> BitArray<E>::fromBinary(const std::string &data) {
        return fromStr<E>(data, 2, 1);
    }

    template <Endian E> BitArray<E> BitArray<E>::fromHex(const std::string &data) {
        return fromStr<E>(data, 16, 4);
    }

    template <Endian E>
    BitArray<E>::BitArray(std::size_t n) : BitArrayView<E>(new std::byte[nBytesToHold(n)], n, 0) {}

    template <Endian E> BitArray<E>::BitArray(std::size_t n, bool value) : BitArray(n) {
        std::memset(
                this->data(), std::to_integer<int>(value ? ~std::byte(0) : std::byte(0)),
                this->nCoveredBytes());
    }

    template <Endian E> BitArray<E>::BitArray(const void *data, std::size_t n) : BitArray(n) {
        std::memcpy(this->data(), data, this->nCoveredBytes());
    }

    template <Endian E>
    BitArray<E>::BitArray(BitArray &&other) : BitArray(other.data(), other.size()) {
        other.m_owning = false;
    }

    template <Endian E>
    template <Endian E2>
    BitArray<E>::BitArray(const ConstBitArrayView<E2> &other) : BitArray(other.size(), 0) {
        auto itr = this->byte_begin();
        auto otherItr = other.aligned_byte_begin();
        auto end = this->byte_end();
        auto otherEnd = other.aligned_byte_end();
        for (; itr != end && otherItr != otherEnd; ++itr, ++otherItr)
            *itr = *otherItr;
        // Zero any remaining bytes
        for (; itr != end; ++itr)
            *itr = std::byte(0);
    }

    template <Endian E> BitArray<E>::~BitArray() {
        if (m_owning)
            delete[] (this->data());
    }

    template class BitArray<Endian::Little>;
    template class BitArray<Endian::Big>;
#ifndef NODOXYGEN
    template BitArray<Endian::Little>::BitArray(const ConstBitArrayView<Endian::Little> &);
    template BitArray<Endian::Little>::BitArray(const ConstBitArrayView<Endian::Big> &);
    template BitArray<Endian::Big>::BitArray(const ConstBitArrayView<Endian::Little> &);
    template BitArray<Endian::Big>::BitArray(const ConstBitArrayView<Endian::Big> &);
#endif

} // namespace Bytestream