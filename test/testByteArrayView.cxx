#define BOOST_TEST_MODULE BytestreamByteArrayView
#include "Bytestream/ByteArrayView.h"
#include "boost/test/included/unit_test.hpp"
#include <algorithm>
#include <cstddef>
#include <vector>

#include "Bytestream/StringUtils.h"

BOOST_AUTO_TEST_CASE(modifyView) {
    std::vector<std::byte> data(10, std::byte{0});
    Bytestream::ByteArrayView view(data.data(), data.size());

    view[0] = std::byte{0x3f};
    BOOST_CHECK(data.back() == std::byte{0x3f});

    std::vector<std::byte> data2{std::byte{0x2f}, std::byte{0xd8},
                                 std::byte{0xa5}};
    std::copy(data2.begin(), data2.end(), view.begin() + 3);
    BOOST_CHECK(data[6] == std::byte{0x2f});
    BOOST_CHECK(data[5] == std::byte{0xd8});
    BOOST_CHECK(data[4] == std::byte{0xa5});

    Bytestream::ByteArrayView view2 = view.view(6, 4);
    std::vector<std::byte> data3{std::byte{0x3e}, std::byte{0xf6},
                                 std::byte{0x2c}, std::byte{0x90}};
    std::copy(data3.begin(), data3.end(), view2.rbegin());
    for (std::size_t idx = 0; idx < 4; ++idx)
        BOOST_CHECK(data[idx] == data3[idx]);
}