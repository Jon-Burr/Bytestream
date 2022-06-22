#define BOOST_TEST_MODULE BytestreamReadBytestreamHelpers
#include "Bytestream/ByteArray.h"
#include "Bytestream/ReadBytestream.h"
#include "Bytestream/ReadBytestreamHelpers.h"
#include "boost/test/included/unit_test.hpp"
#include <boost/test/tools/floating_point_comparison.hpp>

BOOST_AUTO_TEST_CASE(readHelpers) {
    Bytestream::ByteArray data = Bytestream::ByteArray::fromHex(
            "1a 00 00 00 c9 02 00 00 aa 72 20 b8 ab cd b8 44 da 43 a2 01 98 cc "
            "62 b8 aa aa");
    Bytestream::ReadBytestream stream = data;
    Bytestream::ExpectsHandle prefix(Bytestream::ByteArray::fromHex("aa aa"));
    uint8_t elinkCount;
    uint32_t commTimeStamp;
    uint8_t nWords;
    Bytestream::FloatHandle<float, uint16_t> leadingTime(0.195, 12);
    Bytestream::FloatHandle<float, uint8_t> timeWidth(0.4, 7);
    uint8_t channel;
    uint8_t tdc;
    std::bitset<4> errorFlag;
    uint8_t elinkCount2;
    Bytestream::ExpectsHandle fixed(Bytestream::ByteArray::fromHex("ab cd"));
    uint8_t elinkCount3;
    uint16_t evtID;
    Bytestream::ExpectsHandle suffix(Bytestream::ByteArray::fromHex("aa"));
    uint32_t elinkNumber;
    uint32_t nBytes;
    stream >> prefix >> elinkCount >>
            Bytestream::makeReader(commTimeStamp, 3 * CHAR_BIT) >> nWords >>
            leadingTime >> timeWidth >> Bytestream::makeReader(channel, 5) >>
            Bytestream::makeReader(tdc, 4) >> errorFlag >> elinkCount2 >>
            fixed >> elinkCount3 >> evtID >> suffix >>
            Bytestream::makeReader(elinkNumber, std::endian::little) >>
            Bytestream::makeReader(nBytes, std::endian::little);
    BOOST_CHECK(stream.exhausted());
    BOOST_CHECK(prefix.good());
    BOOST_CHECK(elinkCount == 184);
    BOOST_CHECK(commTimeStamp == 10013794);
    BOOST_CHECK(nWords == 1);
    BOOST_CHECK_CLOSE(leadingTime.value(), 181.35, 0.0001);
    BOOST_CHECK_CLOSE(timeWidth.value(), 14.4, 0.0001);
    BOOST_CHECK(channel == 27);
    BOOST_CHECK(tdc == 4);
    BOOST_CHECK(errorFlag == 0b0100);
    BOOST_CHECK(elinkCount2 == 184);
    BOOST_CHECK(fixed.good());
    BOOST_CHECK(elinkCount3 == 184);
    BOOST_CHECK(evtID == 29216);
    BOOST_CHECK(suffix.good());
    BOOST_CHECK(elinkNumber == 713);
    BOOST_CHECK(nBytes == 26);
}