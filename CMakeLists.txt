cmake_minimum_required(VERSION 3.20)
project(ozlib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB OZSTD_SOURCES CONFIGURE_DEPENDS "src/spec/*.cpp")
add_library(ozstd STATIC
    ${OZSTD_SOURCES}
)
target_compile_options(ozstd PRIVATE -O2 -Wall -g -fno-exceptions -fno-rtti)
target_include_directories(ozstd PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

file(GLOB COMMONSTD_SOURCES CONFIGURE_DEPENDS "src/common/*.cpp")
add_library(commonstd STATIC
    ${COMMONSTD_SOURCES}
)
target_compile_options(commonstd PRIVATE -O2 -Wall -g -fno-exceptions -fno-rtti)
target_include_directories(commonstd PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        bios_ut
        GIT_REPOSITORY https://github.com/biobios/bios-ut.git
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(bios_ut)

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "test/test_*.cpp")
    add_executable(ozlib_test ${TEST_SOURCES})
    target_link_libraries(ozlib_test PRIVATE bios::ut_main commonstd)
    add_test(NAME ozlib_test COMMAND ozlib_test)
endif()
