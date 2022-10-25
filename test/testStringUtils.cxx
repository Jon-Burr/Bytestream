//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BytestreamStringUtils
#include "Bytestream/Endian.h"
#include "Bytestream/StringUtils.h"
#include "Bytestream/Utils.h"
#include <boost/test/included/unit_test.hpp>

#include <bit>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <vector>

BOOST_AUTO_TEST_CASE(hex) {
    // Make sure we treat an empty string correctly
    BOOST_CHECK(Bytestream::fromHex("").size() == 0);
    BOOST_CHECK(Bytestream::fromHex("  \t\n ").size() == 0);
    // Read a 4 bytes from a string
    std::vector<std::byte> bytes = Bytestream::fromHex("23afe405");
    std::vector<std::byte> testVal = {
            std::byte{0x23}, std::byte{0xaf}, std::byte{0xe4}, std::byte{0x05}};
    BOOST_CHECK(bytes == testVal);
    // Make sure we can have as many spaces as we like between bytes
    BOOST_CHECK(testVal == Bytestream::fromHex("  23 afe4     05  "));
    // Test reading a non-standard number of bytes
    bytes = Bytestream::fromHex("43 a2 f3 03 00 97 c6");
    testVal = {std::byte{0x43}, std::byte{0xa2}, std::byte{0xf3}, std::byte{0x03},
               std::byte{0x00}, std::byte{0x97}, std::byte{0xc6}};
    BOOST_CHECK(testVal == bytes);
    // Now make sure we get the errors we expect from bad strings
    // Check for a space in a byte
    BOOST_CHECK_THROW(Bytestream::fromHex("43a 2f3"), std::invalid_argument);
    // Check for an invalid character
    BOOST_CHECK_THROW(Bytestream::fromHex("ag"), std::invalid_argument);
    // Make sure that the return trip works OK
    BOOST_CHECK(Bytestream::toHex(testVal.data(), testVal.size()) == "43 a2 f3 03 00 97 c6");
    // Make sure we can handle an empty value
    BOOST_CHECK(Bytestream::toHex(testVal.data(), 0) == "");
    // Check conversion from a value
    uint32_t val = 0x3192d2fau;
    if (Bytestream::Endian::Native != Bytestream::Endian::Big)
        val = Bytestream::swapEndianness(val);
    BOOST_CHECK(Bytestream::toHex(val) == "31 92 d2 fa");
}

BOOST_AUTO_TEST_CASE(binary) {
    // Make sure we treat an empty string correctly
    BOOST_CHECK(Bytestream::fromBinary("").size() == 0);
    BOOST_CHECK(Bytestream::fromBinary(" \t\n  ").size() == 0);
    // Read 2 bytes from a string
    std::vector<std::byte> bytes = Bytestream::fromBinary("0101100111000101");
    std::vector<std::byte> testVal{std::byte{0b01011001}, std::byte{0b11000101}};
    BOOST_CHECK(testVal == bytes);
    // Make sure having spaces is OK
    BOOST_CHECK(testVal == Bytestream::fromBinary("\t 01011001\n 11000101   "));
    // Make sure it chokes on bad format
    BOOST_CHECK_THROW(Bytestream::fromBinary("0101 1100"), std::invalid_argument);
    BOOST_CHECK_THROW(Bytestream::fromBinary("12f31000"), std::invalid_argument);
    // Make sure we can go back to a string
    BOOST_CHECK(Bytestream::toBinary(bytes.data(), bytes.size()) == "01011001 11000101");
    // Make sure we can convert a value
    BOOST_CHECK(Bytestream::toBinary(std::byte{0b10010101}) == "10010101");
}