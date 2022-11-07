#include "Bytestream/PackedBlock.h"
#include "Bytestream/Utils.h"

namespace {
    std::size_t nBitsForFields(const std::vector<std::pair<std::string, std::size_t>> &fields) {
        std::size_t total = 0;
        for (const auto &p : fields)
            total += p.second;
        return total;
    }
} // namespace

namespace Bytestream {
    PackedBlock::PackedBlock(const std::vector<std::pair<std::string, std::size_t>> &fields)
            : m_data(nBytesToHold(nBitsForFields(fields))) {
        std::size_t position = 0;
        for (const auto &p : fields) {
            m_fieldNames.push_back(p.first);
            m_nameToIdx[p.first] = m_positions.size();
            m_positions.push_back(position);
            position += p.second;
        }
        m_positions.push_back(position);
    }

    std::size_t PackedBlock::fieldIdx(const std::string &field) const {
        return m_nameToIdx.at(field);
    }

    const std::string &PackedBlock::field(std::size_t idx) const { return m_fieldNames.at(idx); }

    std::size_t PackedBlock::fieldSize(const std::string &field) const {
        return fieldSize(fieldIdx(field));
    }

    std::size_t PackedBlock::fieldSize(std::size_t idx) const {
        return m_positions.at(idx + 1) - m_positions.at(idx);
    }

    std::size_t PackedBlock::fieldPosition(const std::string &field) const {
        return fieldPosition(fieldIdx(field));
    }

    std::size_t PackedBlock::fieldPosition(std::size_t idx) const { return m_positions.at(idx); }

    std::size_t PackedBlock::size() const { return m_fieldNames.size(); }

    std::size_t PackedBlock::nBits() const { return m_positions.back(); }

    const std::vector<std::string> &PackedBlock::fieldNames() const { return m_fieldNames; }

    ByteArray PackedBlock::operator[](const std::string &field) const {
        return (*this)[fieldIdx(field)];
    }

    ByteArray PackedBlock::at(const std::string &field) const { return at(fieldIdx(field)); }

    ByteArray PackedBlock::operator[](std::size_t idx) const { return at(idx); }

    ByteArray PackedBlock::at(std::size_t idx) const {
        ByteArray data = ByteArray(nBytesToHold(fieldSize(idx)));
        m_data.readBitsInto(
                data.data(), fieldPosition(idx), fieldSize(idx), Endian::Big, Endian::Big);
        return data;
    }
} // namespace Bytestream