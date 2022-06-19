
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "Bytestream/StringUtils.h"
#include "Bytestream/Utils.h"

int main()
{
    uint16_t target{0};
    uint32_t source{0x01234567ul};
    std::cout << Bytestream::toBinary(source) << std::endl;
    std::cout << Bytestream::toHex(source) << std::endl;
    std::cout << Bytestream::toHex(Bytestream::swapEndianness(source))
              << std::endl;
    std::vector<std::byte> bytes = Bytestream::fromHex("23afe405");
    bytes = Bytestream::fromHex("01 a,");
    std::cout << Bytestream::toHex(bytes.data(), bytes.size()) << std::endl;
}