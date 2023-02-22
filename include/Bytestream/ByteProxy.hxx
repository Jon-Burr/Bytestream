/**
 * @file ByteProxy.hxx
 * @brief Proxy for an offset byte
 */

#ifndef BYTESTREAM_BYTEPROXY_HXX
#define BYTESTREAM_BYTEPROXY_HXX

#include <climits>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace Bytestream {

    /// @brief Proxy for a represented byte value which is spread over two underlying bytes
    /// @tparam Iterator The underlying iterator type
    ///
    /// A truncated byte (one with fewer than CHAR_BIT bits) can also be represented. In this case
    /// the out of range bits will be 0 in the returned value
    template <typename Iterator> class ConstByteProxy {
    public:
        /// @brief Create the proxy
        /// @param itr The iterator to the byte containing the least significant bits
        /// @param offset The offset within the underlying memory
        /// @param size The number of bits contained
        ConstByteProxy(Iterator itr, uint8_t offset, uint8_t size = CHAR_BIT);
        virtual ~ConstByteProxy() = default;

        /// @brief Retrieve the value as a std::byte
        operator std::byte() const { return get(); }
        /// @brief Retrieve the value as a std::byte
        std::byte get() const;

    protected:
        /// The iterator to the byte containing the least significant bits
        Iterator m_itr;
        /// The offset within the underlying memory
        uint8_t m_offset;
        /// The number of bits contained
        uint8_t m_size;
    };

    /// @brief Mutable proxy for a represented byte value which is spread over two underlying bytes
    /// @tparam Iterator The underlying iterator type
    ///
    /// This proxy can be assigned to, modifying the underlying memory as if it were correctly
    /// aligned. Any bits that are out of range are not changed.
    template <typename Iterator> class ByteProxy : public ConstByteProxy<Iterator> {
    public:
        using ConstByteProxy<Iterator>::ConstByteProxy;

        /// @brief Set the underlying memory
        ByteProxy &operator=(std::byte b) {
            set(b);
            return *this;
        }
        /// @brief Set the underlying memory
        void set(std::byte b);
    };

    /// @brief Create a byte proxy
    /// @param itr The iterator to the byte containing the least significant bits
    /// @param offset The offset within the underlying memory
    /// @param size The number of bits contained
    inline ByteProxy<std::byte *> makeByteProxy(
            std::byte *itr, uint8_t offset, uint8_t size = CHAR_BIT) {
        return {itr, offset, size};
    }

    /// @brief Create a byte proxy
    /// @param itr The iterator to the byte containing the least significant bits
    /// @param offset The offset within the underlying memory
    /// @param size The number of bits contained
    inline ByteProxy<std::reverse_iterator<std::byte *>> makeByteProxy(
            std::reverse_iterator<std::byte *> itr, uint8_t offset, uint8_t size = CHAR_BIT) {
        return {itr, offset, size};
    }

    /// @brief Create a byte proxy
    /// @param itr The iterator to the byte containing the least significant bits
    /// @param offset The offset within the underlying memory
    /// @param size The number of bits contained
    inline ConstByteProxy<const std::byte *> makeByteProxy(
            const std::byte *itr, uint8_t offset, uint8_t size = CHAR_BIT) {
        return {itr, offset, size};
    }

    /// @brief Create a byte proxy
    /// @param itr The iterator to the byte containing the least significant bits
    /// @param offset The offset within the underlying memory
    /// @param size The number of bits contained
    inline ConstByteProxy<std::reverse_iterator<const std::byte *>> makeByteProxy(
            std::reverse_iterator<const std::byte *> itr, uint8_t offset, uint8_t size = CHAR_BIT) {
        return {itr, offset, size};
    }

    namespace detail {
        /// @brief The correct proxy type based on the provided byte iterator
        /// @tparam Iterator The byte iterator type
        template <typename Iterator>
        using iterator_proxy_t =
                decltype(makeByteProxy(std::declval<Iterator>(), uint8_t(), uint8_t()));
    } // namespace detail

} // namespace Bytestream

#endif //> !BYTESTREAM_BYTEPROXY_HXX