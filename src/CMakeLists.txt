add_library(Bytestream)
target_sources(Bytestream PRIVATE
    ByteArray.cxx
    ByteArrayView.cxx
    ConstByteArrayView.cxx
    ReadBytestream.cxx
    ReadBytestreamHelpers.cxx
    ReadUtils.cxx
    StringUtils.cxx
    Utils.cxx
)
target_include_directories(Bytestream PUBLIC ../include)
target_compile_features(Bytestream PUBLIC cxx_std_20)