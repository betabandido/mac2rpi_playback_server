cmake_minimum_required(VERSION 3.13)
project(mac2rpi_playback_server)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(mac2rpi_playback_server server.cpp)
    target_link_libraries(mac2rpi_playback_server ${Boost_LIBRARIES} Threads::Threads)
endif()
