find_package(Boost COMPONENTS log system unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK -DBOOST_LOG_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS} ../src)
add_executable(Test test.cpp)
target_link_libraries(
        Test
        ${Boost_LIBRARIES}
)

add_test(NAME MyTest COMMAND Test)
