cmake_minimum_required(VERSION 3.0.0)
project(bn VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0135 NEW)

# See: https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/518387203b573f35477fa6872dd54620e70d2bdb.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(bn src/main.cpp src/graph.cpp src/graph.hpp)

target_compile_options(bn PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall>
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4>)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

enable_testing()

add_executable(
    tests
    src/graph.test.cpp
)

target_link_libraries(
    tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
