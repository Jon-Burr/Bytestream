cmake_minimum_required(VERSION 3.12)

project(Bytestream VERSION 0.1.0 LANGUAGES CXX)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)