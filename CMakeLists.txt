cmake_minimum_required(VERSION 3.13)
project(HW4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(HW4 test_pqueue.cc pqueue.h)

target_include_directories(HW4
        PRIVATE
        ${GTEST_INCLUDE_DIRS})
target_link_libraries(HW4
        PRIVATE
        GTest::GTest
        Threads::Threads)
