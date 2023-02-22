#include "Bytestream/utils.hxx"
#include <algorithm>
#include <iterator>

// TODO: I'm afraid that a lot of this is wrong, especially for little endian arrays...

namespace Bytestream {
    void swapEndiannessInPlace(void *data, std::size_t size) {
        std::byte *data_ = static_cast<std::byte *>(data);
        for (std::size_t idx = 0; idx < size / 2; ++idx)
            std::swap(*(data_ + idx), *(data_ + size - idx - 1));
    }
} // namespace Bytestream