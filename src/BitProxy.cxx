#include "Bytestream/BitProxy.hxx"

#include <climits>
#include <stdexcept>
#include <string>

namespace Bytestream {
    BitProxy::BitProxy(void *data, uint8_t offset)
            : m_data(static_cast<std::byte *>(data)), m_mask(std::byte(1) << offset) {
        if (offset >= CHAR_BIT)
            throw std::out_of_range(std::to_string(offset));
    }

    BitProxy::BitProxy(void *data, std::byte mask)
            : m_data(static_cast<std::byte *>(data)), m_mask(mask) {}

    BitProxy &BitProxy::operator=(bool value) {
        if (value)
            *m_data |= m_mask;
        else
            *m_data &= ~m_mask;
        return *this;
    }

    BitProxy::operator bool() const { return bool(*m_data & m_mask); }
} // namespace Bytestream