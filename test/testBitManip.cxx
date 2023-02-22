#include "Bytestream/utils.hxx"
#define BOOST_TEST_MODULE BytestreamBitManip
#include <boost/test/included/unit_test.hpp>
#include <type_traits>

BOOST_AUTO_TEST_CASE(bitsToBytes) {
    BOOST_CHECK(Bytestream::nBytesToHold(3) == 1);
    BOOST_CHECK(Bytestream::nBytesToHold(8) == 1);
    BOOST_CHECK(Bytestream::nBytesToHold(20) == 3);
    BOOST_CHECK(Bytestream::nBytesToHold(104) == 13);
}

BOOST_AUTO_TEST_CASE(smallestUInts) {
    BOOST_CHECK((std::is_same_v<typename Bytestream::BytesInfo<3>::uint_t, uint32_t>));
    BOOST_CHECK(Bytestream::BytesInfo<5>::nBytes == 8);
    BOOST_CHECK(Bytestream::BitsInfo<9>::nBytes == 2);
    BOOST_CHECK((std::is_same_v<typename Bytestream::BitsInfo<5>::uint_t, uint8_t>));
}

BOOST_AUTO_TEST_CASE(swapEndianness) {
    BOOST_CHECK(Bytestream::reverseBits(std::byte{0b00101101}) == std::byte{0b10110100});
    BOOST_CHECK(Bytestream::reverseBits(std::byte{0b00001111}) == std::byte{0b11110000});
    BOOST_CHECK(Bytestream::swapEndianness(uint8_t(0xa2)) == 0xa2);
    BOOST_CHECK(Bytestream::swapEndianness(uint16_t(0xb200)) == 0x00b2);
    BOOST_CHECK(Bytestream::swapEndianness(uint16_t(0x1e34)) == 0x341e);
    BOOST_CHECK(Bytestream::swapEndianness(uint32_t(0x33f2a3)) == 0xa3f23300);
    BOOST_CHECK(Bytestream::swapEndianness(uint64_t(0x43ab65f289cd009e)) == 0x9e00cd89f265ab43);
    uint32_t value = 0xa3f23300;
    Bytestream::swapEndiannessInPlace(reinterpret_cast<std::byte *>(&value) + 1, 3);
    BOOST_CHECK(value == 0x33f2a300);
}

BOOST_AUTO_TEST_CASE(masks) {
    BOOST_CHECK(Bytestream::createLeftMask(3) == std::byte(0b11100000));
    BOOST_CHECK(Bytestream::createLeftMask(9) == std::byte(0b11111111));
    BOOST_CHECK(Bytestream::createLeftMask(6) == std::byte(0b11111100));
    BOOST_CHECK(Bytestream::createRightMask(2) == std::byte(0b11));
    BOOST_CHECK(Bytestream::createRightMask(6) == std::byte(0b111111));
    BOOST_CHECK(Bytestream::createRightMask(9) == std::byte(0b11111111));
}