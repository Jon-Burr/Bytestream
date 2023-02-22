/// @file BitProxy.hxx
/// @brief Proxy allowing modifying individual bits in an array

#ifndef BYTESTREAM_BITPROXY_HXX
#define BYTESTREAM_BITPROXY_HXX

#include <cstddef>
#include <cstdint>

namespace Bytestream {
    /// @brief Mutable proxy for a single bit
    class BitProxy {
    public:
        /// @brief Create the proxy
        /// @param data Pointer to the byte to be modified
        /// @param offset Offset within the byte, must be less than CHAR_BIT
        BitProxy(void *data, uint8_t offset);
        /// @brief Create the proxy
        /// @param data Pointer to the byte to be modified
        /// @param mask The byte mask
        BitProxy(void *data, std::byte mask);
        BitProxy(const BitProxy &) = delete;

        /// @brief Set the bit
        BitProxy &operator=(bool value);

        /// @brief Get the value of the bit
        operator bool() const;

    private:
        std::byte *m_data;
        std::byte m_mask;
    }; //> end class BitProxy
} // namespace Bytestream

#endif //> !BYTESTREAM_BITPROXY_HXX