cmake_minimum_required(VERSION 3.0.0)
project(bn VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(bn src/main.cpp src/graph.cpp src/graph.hpp)

target_compile_options(bn PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
