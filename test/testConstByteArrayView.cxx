#define BOOST_TEST_MODULE BytestreamConstByteArrayView
#include "Bytestream/ConstByteArrayView.h"
#include "Bytestream/Endian.h"
#include "boost/test/included/unit_test.hpp"

#include <cstddef>
#include <vector>

// Test basic access to bytes in a view
BOOST_AUTO_TEST_CASE(basicAccess) {
    std::vector<std::byte> data{
            std::byte{0x02}, std::byte{0x43}, std::byte{0xae}, std::byte{0xc7}, std::byte{0xbd}};
    Bytestream::ConstByteArrayView view(data.data(), data.size());
    BOOST_CHECK(view.size() == 5);
    BOOST_CHECK(view.at(3) == std::byte{0x43});
    BOOST_CHECK(view[1] == std::byte{0xc7});
    // Now check iteration
    auto viewItr = view.begin();
    auto vecItr = data.rbegin();
    for (; viewItr != view.end(); ++viewItr, ++vecItr)
        BOOST_CHECK(*viewItr == *vecItr);
    auto viewRItr = view.rbegin();
    auto vecRItr = data.begin();
    for (; viewRItr != view.rend(); ++viewRItr, ++vecRItr)
        BOOST_CHECK(*viewRItr == *vecRItr);
    BOOST_CHECK(view.view(1, 2) == Bytestream::ConstByteArrayView(data.data() + 2, 2));
}

// Test reading bits from the array into other memory
BOOST_AUTO_TEST_CASE(interpretBits) {
    std::vector<std::byte> data{
            std::byte{0x02}, std::byte{0x43}, std::byte{0xae}, std::byte{0xc7}, std::byte{0xbd}};
    Bytestream::ConstByteArrayView view(data.data(), data.size());
    uint8_t value8;
    view.readBytes(value8, 2, 1);
    BOOST_CHECK(value8 == 0xae);
    uint16_t value16;
    view.readBytes(value16, 3, 1);
    BOOST_CHECK(value16 == 0x43);
    view.readBytes(value16, 3, 2);
    BOOST_CHECK(value16 == 0x0243);
    view.readBits(value8, 0, 4);
    BOOST_CHECK(value8 == 0xd);
    view.readBits(value8, 1, 3);
    BOOST_CHECK(value8 == 0b110);
    uint32_t value32;
    view.readBits(value32, 3);
    BOOST_CHECK(value32 == 0b01001000'01110101'11011000'11110111);
    view.readBits(value32, 0, 32, Bytestream::Endian::Little);
    BOOST_CHECK(value32 == 0xbdc7ae43);

    std::bitset<7> bits = view.readBitset<7>(10);
    BOOST_CHECK(bits == std::bitset<7>(0b0110001));
}