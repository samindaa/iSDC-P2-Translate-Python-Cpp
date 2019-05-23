cmake_minimum_required(VERSION 3.14)
project(translate)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "*.cpp")
set(INCLUDES ".")
list(REMOVE_DUPLICATES INCLUDES)
include_directories(${INCLUDES})

file(COPY maps/half_red.txt DESTINATION maps)
file(COPY maps/m1.txt DESTINATION maps)
file(COPY maps/m2.txt DESTINATION maps)
file(COPY maps/m3.txt DESTINATION maps)
file(COPY maps/m4.txt DESTINATION maps)

add_executable(translate ${SOURCES})