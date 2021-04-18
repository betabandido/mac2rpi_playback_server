find_package(Boost COMPONENTS system unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS} ../src)
add_executable(Test test.cpp)
target_link_libraries(
        Test
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(NAME MyTest COMMAND Test)
