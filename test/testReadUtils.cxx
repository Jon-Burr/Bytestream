#include "Bytestream/ReadUtils.h"
#include "Bytestream/Utils.h"
#define BOOST_TEST_MODULE BytestreamReadUtils
#include <bit>
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "Bytestream/StringUtils.h"

BOOST_AUTO_TEST_CASE(createMask)
{
    BOOST_CHECK(Bytestream::createLeftMask(3) == std::byte{0b11100000});
    BOOST_CHECK(Bytestream::createLeftMask(7) == std::byte{0b11111110});
    BOOST_CHECK(Bytestream::createLeftMask(20) == std::byte{0b11111111});
    BOOST_CHECK(Bytestream::createRightMask(2) == std::byte{0b00000011});
    BOOST_CHECK(Bytestream::createRightMask(5) == std::byte{0b00011111});
    BOOST_CHECK(Bytestream::createRightMask(30) == std::byte{0b11111111});
}

BOOST_AUTO_TEST_CASE(copyAndShift)
{
    std::vector<std::byte> source = Bytestream::fromHex("01 23 45 67");
    std::vector<std::byte> target(3);
    // Test copying from a byte boundary
    Bytestream::copyBits(target.data(), source.data(), CHAR_BIT, 3 * CHAR_BIT);
    BOOST_CHECK(Bytestream::toHex(target.data(), target.size()) == "23 45 67");

    // Test copying a right-aligned piece of memory that needs masking
    target.assign(2, std::byte{0});
    source = Bytestream::fromBinary("11101101 11011010 00011111 01101100");
    Bytestream::copyBits(target.data(), source.data(), 2, 14);
    BOOST_CHECK(
        Bytestream::toBinary(target.data(), target.size()) ==
        "00101101 11011010");

    // Make sure the other bytes aren't touched
    target.assign(3, std::byte{0xFF});
    Bytestream::copyBits(target.data() + 1, source.data(), CHAR_BIT + 3, 11);
    BOOST_CHECK(
        Bytestream::toBinary(target.data(), target.size()) ==
        "11111111 00000110 10000111");
    target.assign(3, std::byte{0xFF});
    Bytestream::copyBits(target.data(), source.data(), 1, 2 * CHAR_BIT);
    BOOST_CHECK(
        Bytestream::toBinary(target.data(), target.size()) ==
        "11011011 10110100 11111111");
}