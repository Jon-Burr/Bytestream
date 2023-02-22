#include "Bytestream/utils.hxx"
#include <algorithm>
#include <iterator>

// TODO: I'm afraid that a lot of this is wrong, especially for little endian arrays...

namespace {
    using namespace Bytestream;
    void copyLeft(
            std::reverse_iterator<std::byte *> dest, std::reverse_iterator<const std::byte *> src,
            uint8_t offset, uint8_t shift, std::size_t nBits) {
        if (offset + shift + nBits <= CHAR_BIT) {
            // Working with a single byte on either end
            std::byte srcData = *src << shift;
            std::byte mask = createRightMask(nBits) << offset;
            copyPartialByte(*dest, srcData, mask);
        }
    }

    void copyRight(
            std::reverse_iterator<std::byte *> dest, std::reverse_iterator<const std::byte *> src,
            uint8_t offset, uint8_t shift, std::size_t nBits) {
        if (offset + nBits <= CHAR_BIT) {
            // Working with a single byte on either end
            std::byte srcData = *src >> shift;
            std::byte mask = createRightMask(nBits) << offset;
            copyPartialByte(*dest, srcData, mask);
        }
    }

    void copyParallel(
            std::reverse_iterator<std::byte *> dest, std::reverse_iterator<const std::byte *> src,
            uint8_t offset, std::size_t nBits) {
        // Check if everything is contained within a single byte
        if (offset + nBits <= CHAR_BIT) {
            std::byte mask = createRightMask(nBits) << offset;
            copyPartialByte(*dest, *src, mask);
        } else {
            if (offset != 0) {
                // Incomplete bytes are harder
                copyPartialByte(*dest, *src, createLeftMask(CHAR_BIT - offset));
                // We've copied those bits so advance the pointers and reduce the number of bits
                // left to copy
                ++dest;
                ++src;
                nBits -= (CHAR_BIT - offset);
            }
            // Normally copy any full bytes
            std::size_t nFull = nBits / CHAR_BIT;
            uint8_t remaining = nBits % CHAR_BIT;
            std::memcpy((dest + nFull).base(), (src + nFull).base(), nFull);
            if (remaining != 0)
                copyPartialByte(*(dest + nFull), *(src + nFull), createRightMask(remaining));
        }
    }

} // namespace

namespace Bytestream {
    void swapEndiannessInPlace(void *data, std::size_t size) {
        std::byte *data_ = static_cast<std::byte *>(data);
        for (std::size_t idx = 0; idx < size / 2; ++idx)
            std::swap(*(data_ + idx), *(data_ + size - idx - 1));
    }

    void copyBits(
            void *dest_, std::size_t nDestBytes, uint8_t destOffset, const void *src_,
            std::size_t nSrcBytes, uint8_t srcOffset, std::size_t nBits) {
        if (destOffset >= CHAR_BIT || srcOffset >= CHAR_BIT)
            // If either offset is at least CHAR_BIT offset the end points of the arrays and
            // make the copy
            return copyBits(
                    dest_, nDestBytes - destOffset / CHAR_BIT, destOffset % CHAR_BIT, src_,
                    nSrcBytes - srcOffset / CHAR_BIT, srcOffset % CHAR_BIT, nBits);

        auto dest = std::make_reverse_iterator(static_cast<std::byte *>(dest_) + nDestBytes);
        auto src = std::make_reverse_iterator(static_cast<const std::byte *>(src_) + nSrcBytes);
        if (destOffset == srcOffset)
            copyParallel(dest, src, destOffset, nBits);
        else if (destOffset < srcOffset)
            copyRight(dest, src, destOffset, srcOffset - destOffset, nBits);
        else
            copyLeft(dest, src, destOffset, destOffset - srcOffset, nBits);
        if (destOffset == srcOffset) {
            // If the two arrays are parallel then the process is much simpler
            // Check if everything is contained within a single byte
            if (destOffset + nBits <= CHAR_BIT) {
                std::byte mask = createRightMask(nBits) << destOffset;
                copyPartialByte(*dest, *src, mask);
            } else {
                if (destOffset != 0) {
                    // Incomplete bytes are harder
                    copyPartialByte(*dest, *src, createLeftMask(CHAR_BIT - destOffset));
                    // We've copied those bits so advance the pointers and reduce the number of
                    // bits left to copy
                    ++dest;
                    ++src;
                    nBits -= (CHAR_BIT - destOffset);
                }
                // Normally copy any full bytes
                std::size_t nFull = nBits / CHAR_BIT;
                uint8_t remaining = nBits % CHAR_BIT;
                std::memcpy((dest + nFull).base(), (src + nFull).base(), nFull);
                if (remaining != 0)
                    copyPartialByte(*(dest + nFull), *(src + nFull), createRightMask(remaining));
            }
        } else {
            // Now we have non-parallel offsets, things are fun...
            if (destOffset + nBits <= CHAR_BIT && srcOffset + nBits <= CHAR_BIT) {
                // Working with a single byte on either end
                std::byte srcData = *src;
                if (destOffset > srcOffset)
                    srcData <<= (destOffset - srcOffset);
                else
                    srcData >>= (srcOffset - destOffset);
                std::byte mask = createRightMask(nBits) << destOffset;
                copyPartialByte(*dest, srcData, mask);
            } else {
                if (destOffset != 0) {
                }
            }
        }
    }
} // namespace Bytestream