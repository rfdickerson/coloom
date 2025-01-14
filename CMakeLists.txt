cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(columnar_db)

find_package(Snappy REQUIRED)
find_package(GTest REQUIRED)

add_library(columnar_db src/columnar_db.cpp)
target_link_libraries(columnar_db PRIVATE Snappy::snappy)

add_executable(test_columnar_db tests/test_columnar_db.cpp)
target_link_libraries(test_columnar_db PRIVATE GTest::gtest GTest::gtest_main)
