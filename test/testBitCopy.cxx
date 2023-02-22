#include "Bytestream/utils.hxx"
#define BOOST_TEST_MODULE BytestreamBitCopy
#include <bitset>
#include <boost/test/included/unit_test.hpp>
#include <vector>

namespace {
    std::ostream &operator<<(std::ostream &os, std::byte b) {
        return os << std::bitset<8>(std::to_integer<uint8_t>(b));
    }
    std::ostream &operator<<(std::ostream &os, const std::vector<std::byte> &v) {
        for (std::byte b : v)
            os << b;
        return os;
    }
} // namespace

BOOST_AUTO_TEST_CASE(singleBit) {
    std::byte b1{0b00111100};
    std::byte b2{0b11000011};
    Bytestream::copyBits(&b1, 1, 0, &b2, 1, 0, 2);
    std::cout << b1 << std::endl;
    BOOST_CHECK(b1 == std::byte(0b00111111));
    Bytestream::copyBits(&b1, 1, 2, &b2, 1, 2, 1);
    BOOST_CHECK(b1 == std::byte(0b00111011));
}

BOOST_AUTO_TEST_CASE(parallelArrays) {
    std::vector<std::byte> v1{
            std::byte{0b00101100}, std::byte{0b11011011}, std::byte{0b00001111},
            std::byte{0b11110000}};
    std::vector<std::byte> v2{
            std::byte{0b10110011}, std::byte{0b00101101}, std::byte{0b11110000},
            std::byte{0b00110110}};
    Bytestream::copyBits(v1.data(), v1.size(), 5, v2.data(), v2.size(), 5, 25);
    std::vector<std::byte> result{
            std::byte{0b00110011}, std::byte{0b00101101}, std::byte{0b11110000},
            std::byte{0b00110000}};
    BOOST_CHECK(v1 == result);
}