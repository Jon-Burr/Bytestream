add_library(Bytestream)
target_sources(Bytestream PRIVATE
    AlignedByteIterator.cxx
    BitArray.cxx
    BitArrayView.cxx
    BitProxy.cxx
    ByteProxy.cxx
    ConstBitArrayView.cxx
    ConstBitIterator.cxx
    string.cxx
    utils.cxx
)
target_include_directories(Bytestream PUBLIC ../include)
target_compile_features(Bytestream PUBLIC cxx_std_17)