cmake_minimum_required(VERSION 3.16)
project(parse_ply_cmake_project)

add_library(parse_ply
    parse_ply.hpp
    parse_ply.cpp
)
target_compile_features(parse_ply PRIVATE cxx_std_17)
set_target_properties(parse_ply PROPERTIES CXX_EXTENSIONS OFF)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE parse_ply)
target_compile_features(main PRIVATE cxx_std_17)
set_target_properties(main PROPERTIES CXX_EXTENSIONS OFF)
