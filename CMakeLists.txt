cmake_minimum_required(VERSION 3.20.0)
project(Compiler_Principles VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

# include(FetchContent)
# FetchContent_Declare(
# googleTest
# GIT_REPOSITORY https://github.com/google/googletest.git
# GIT_TAG        v1.14.0
# )

# FetchContent_MakeAvailable(googleTest)


# include_directories("${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(src)
add_subdirectory(test)

# git submodule add git@github.com:google/googletest.git third_party/googletest
# git submodule update
# add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/googletest")