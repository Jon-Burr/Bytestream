find_package(Boost COMPONENTS unit_test_framework REQUIRED) # TODO: Specify version?

function(new_test TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_NAME}.cxx)
    target_link_libraries(${TEST_NAME} PRIVATE Bytestream Boost::unit_test_framework)
    add_test(${TEST_NAME} ${TEST_NAME})
endfunction()

new_test(testBitManip)