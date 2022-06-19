find_package(Boost COMPONENTS unit_test_framework REQUIRED) # TODO: Specify version?

add_executable(testUtils testUtils.cxx)
target_link_libraries(testUtils PRIVATE Bytestream Boost::unit_test_framework)
add_test(testUtils testUtils)

add_executable(testReadUtils testReadUtils.cxx)
target_link_libraries(testReadUtils PRIVATE Bytestream Boost::unit_test_framework)
add_test(testReadUtils testReadUtils)

add_executable(testStringUtils testStringUtils.cxx)
target_link_libraries(testStringUtils PRIVATE Bytestream Boost::unit_test_framework)
add_test(testStringUtils testStringUtils)