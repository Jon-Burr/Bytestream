workspace "Bytestream"
    configurations "Release"

project "BytestreamLib"
    kind "SharedLib"
    language "C++"
    targetdir "lib"
    files {"include/Bytestream/*.h", "src/*.cxx"}
    includedirs "include"
    buildoptions "-std=c++17"