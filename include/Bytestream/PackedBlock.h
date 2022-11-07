/**
 * @file PackedBlock.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Block containing named data items at specific bit positions
 * @version 0.1
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 */

#ifndef BYTESTREAM_PACKEDBLOCK_H
#define BYTESTREAM_PACKEDBLOCK_H

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Bytestream/ByteArray.h"

namespace Bytestream {
    class PackedBlock {
    public:
        /**
         * @brief Create the block
         * @param fields The field specifiers
         *
         * The fields are specified as a list of (name, size) pairs, where size is the number of
         * bits used to store that field.
         */
        PackedBlock(const std::vector<std::pair<std::string, std::size_t>> &fields);

        /// @brief Get the index of the given field
        std::size_t fieldIdx(const std::string &field) const;

        /// @brief Get the name of the field at the given index
        const std::string &field(std::size_t idx) const;

        /// @brief Get the size (in bits) of the given field
        std::size_t fieldSize(const std::string &field) const;

        /// @brief Get the size (in bits) of the field at the given index
        std::size_t fieldSize(std::size_t idx) const;

        /// @brief Get the position (in bits from the right) of the given field
        std::size_t fieldPosition(const std::string &field) const;

        /// @brief Get the position (in bits from the right) of the field at the given index
        std::size_t fieldPosition(std::size_t idx) const;

        /// @brief The number of fields
        std::size_t size() const;

        /// @brief The total size in bits
        std::size_t nBits() const;

        /// @brief Get the names of all the fields
        const std::vector<std::string> &fieldNames() const;

        /**
         * @brief Read the data in the specified field
         */
        ByteArray operator[](const std::string &field) const;
        ByteArray at(const std::string &field) const;
        ByteArray operator[](std::size_t idx) const;
        ByteArray at(std::size_t idx) const;

        /**
         * @brief Get the (typed) value in the specified field
         */
        template <typename T = uint32_t>
        std::enable_if_t<is_uint_v<T>, T> get(const std::string &field) const;
        template <typename T = uint32_t>
        std::enable_if_t<is_uint_v<T>, T> get(std::size_t idx) const;

        /**
         * @brief Get the value stored in the specified field as a bitset
         */
        template <std::size_t N> std::bitset<N> getBits(const std::string &field) const;
        template <std::size_t N> std::bitset<N> getBits(std::size_t idx) const;

        template <typename T>
        std::enable_if_t<is_uint_v<T>, void> set(const std::string &field, const T &value);
        template <typename T>
        std::enable_if_t<is_uint_v<T>, void> set(std::size_t idx, const T &value);

    private:
        ByteArray m_data;
        std::vector<std::string> m_fieldNames;
        std::map<std::string, std::size_t> m_nameToIdx;
        std::vector<std::size_t> m_positions;
    }; //> end class PackedBlock
} // namespace Bytestream

#include "Bytestream/PackedBlock.icc"

#endif //> !BYTESTREAM_PACKEDBLOCK_H