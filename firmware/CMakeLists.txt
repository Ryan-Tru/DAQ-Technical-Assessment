cmake_minimum_required(VERSION 3.22)

project(firmware)

add_subdirectory(dbcppp)
add_subdirectory(Catch2)
add_subdirectory(solution)
add_subdirectory(tests)

target_link_libraries(answer PRIVATE dbcppp)
target_link_libraries(tests PRIVATE Catch2 dbcppp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dbcppp/include)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/solution
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove -G "${CMAKE_GENERATOR}" cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove -G "${CMAKE_GENERATOR}" Makefile
    COMMAND ${CMAKE_COMMAND} -E remove -G "${CMAKE_GENERATOR}" DartConfiguration.tcl
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
