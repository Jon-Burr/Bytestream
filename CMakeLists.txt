cmake_minimum_required(VERSION 3.12)

project(Bytestream VERSION 0.2.0 LANGUAGES CXX)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)