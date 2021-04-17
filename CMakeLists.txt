cmake_minimum_required(VERSION 3.13)
project(mac2rpi_playback_server)

set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS system)
# TODO: use find_package for PortAudio cpp bindings
#   when CMake support for them is available.

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(mac2rpi_playback_server server.cpp)
    target_link_libraries(
        mac2rpi_playback_server
        portaudiocpp
        ${Boost_LIBRARIES}
        Threads::Threads
    )
endif()
