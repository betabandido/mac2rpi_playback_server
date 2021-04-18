find_package(Boost COMPONENTS log system REQUIRED)

# TODO: use find_package for PortAudio cpp bindings
#   when CMake support for them is available.
#   See: https://github.com/PortAudio/portaudio/issues/539
#   and https://github.com/PortAudio/portaudio/issues/548

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(mac2rpi_playback_server main.cpp)
    set_target_properties(mac2rpi_playback_server PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(
            mac2rpi_playback_server
             portaudiocpp
            ${Boost_LIBRARIES}
            Threads::Threads
    )
endif ()
