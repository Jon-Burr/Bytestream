find_package(Boost COMPONENTS unit_test_framework REQUIRED) # TODO: Specify version?
add_definitions(-DBOOST_ALL_DYN_LINK)

add_executable(testUtils testUtils.cxx)
target_link_libraries(testUtils PRIVATE Bytestream Boost::unit_test_framework)
add_test(testUtils testUtils)

add_executable(testReadUtils testReadUtils.cxx)
target_link_libraries(testReadUtils PRIVATE Bytestream Boost::unit_test_framework)
add_test(testReadUtils testReadUtils)

add_executable(testStringUtils testStringUtils.cxx)
target_link_libraries(testStringUtils PRIVATE Bytestream Boost::unit_test_framework)
add_test(testStringUtils testStringUtils)

add_executable(testConstByteArrayView testConstByteArrayView.cxx)
target_link_libraries(testConstByteArrayView PRIVATE Bytestream Boost::unit_test_framework)
add_test(testConstByteArrayView testConstByteArrayView)

add_executable(testByteArrayView testByteArrayView.cxx)
target_link_libraries(testByteArrayView PRIVATE Bytestream Boost::unit_test_framework)
add_test(testByteArrayView testByteArrayView)

add_executable(testReadBytestream testReadBytestream.cxx)
target_link_libraries(testReadBytestream PRIVATE Bytestream Boost::unit_test_framework)
add_test(testReadBytestream testReadBytestream)

add_executable(testReadBytestreamHelpers testReadBytestreamHelpers.cxx)
target_link_libraries(testReadBytestreamHelpers PRIVATE Bytestream Boost::unit_test_framework)
add_test(testReadBytestreamHelpers testReadBytestreamHelpers)