#include "Bytestream/Utils.h"
#include <algorithm>

namespace Bytestream {
    void swapEndiannessInPlace(void *data, std::size_t size) {
        std::byte *data_ = reinterpret_cast<std::byte *>(data);
        for (std::size_t idx = 0; idx < size / 2; ++idx)
            std::swap(*(data_ + idx), *(data_ + size - idx - 1));
    }
} // namespace Bytestream