cmake_minimum_required(VERSION 3.22)

project(tests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executable
add_executable(tests tests.cpp)

target_sources(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../solution/canParser.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../solution
    ${CMAKE_CURRENT_SOURCE_DIR}/../dbcppp/include 
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain dbcppp)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
endif()


