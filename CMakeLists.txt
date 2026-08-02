cmake_minimum_required(VERSION 3.20)
project(ozlib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB OZSTD_SOURCES CONFIGURE_DEPENDS "src/spec/*.cpp")
add_library(ozstd STATIC
    ${OZSTD_SOURCES}
)
target_compile_options(ozstd PRIVATE
    --target=x86_64-unknown-none-elf
    -O2
    -Wall
    -g
    -ffreestanding
    -mno-red-zone
    -fno-exceptions
    -fno-rtti
)
target_include_directories(ozstd PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

file(GLOB COMMONSTD_SOURCES CONFIGURE_DEPENDS "src/common/*.cpp")
add_library(commonstd STATIC
    ${COMMONSTD_SOURCES}
)
target_compile_options(commonstd PRIVATE
    --target=x86_64-unknown-none-elf
    -O2
    -Wall
    -g
    -ffreestanding
    -mno-red-zone
    -fno-exceptions
    -fno-rtti
)
target_include_directories(commonstd PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
