#define BOOST_TEST_MODULE BytestreamReadBytestream
#include "Bytestream/ByteArray.h"
#include "Bytestream/ReadBytestream.h"
#include "boost/test/included/unit_test.hpp"

#include "Bytestream/StringUtils.h"
#include <bitset>

BOOST_AUTO_TEST_CASE(readStream) {
    Bytestream::ByteArray data = Bytestream::ByteArray::fromBinary("01101110");
    Bytestream::ReadBytestream stream = data;
    uint8_t value8{0};
    stream.readBits(value8);
    BOOST_TEST(value8 == 0b01101110);
    stream.reset();
    stream >> value8;
    BOOST_TEST(value8 == 0b01101110);
    stream.reset();
    std::bitset<5> bitset1;
    std::bitset<3> bitset2;
    BOOST_TEST(stream.good());
    stream >> bitset1 >> bitset2;
    BOOST_TEST(stream.exhausted());
    BOOST_TEST(bitset1 == 0b01110);
    BOOST_TEST(bitset2 == 0b011);

    Bytestream::ByteArray data2 = Bytestream::ByteArray::fromHex("aa f2 c3 d8 b2 97 42");
    uint32_t value32{0};
    uint16_t value16{0};
    Bytestream::ReadBytestream stream2 = data2;
    stream2 >> value16 >> value32 >> value8;
    BOOST_TEST(value16 == 0x97'42);
    BOOST_TEST(value32 == 0xf2'c3'd8'b2);
    BOOST_TEST(value8 == 0xaa);
}