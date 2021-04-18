cmake_minimum_required(VERSION 3.13)
project(mac2rpi_playback_server)

set(CMAKE_CXX_STANDARD 14)

if (NOT DEFINED ENV{BUILD_ONLY_TESTS})
    add_subdirectory(src)
endif ()

enable_testing()
add_subdirectory(test)
