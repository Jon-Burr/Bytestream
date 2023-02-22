cmake_minimum_required(VERSION 3.12)

project(Bytestream VERSION 0.2.0 LANGUAGES CXX)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)

add_executable(playground playground.cxx)
target_link_libraries(playground PRIVATE Bytestream)