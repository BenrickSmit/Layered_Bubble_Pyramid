cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project("Layered_Bubble_Pyramid")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(warnings "-Wall -Wextra ")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(warnings "/W4 /WX ")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(LayeredBubblePyramid 	main.cpp 
					genetic-algorithm.cpp genetic-algorithm.h 
					layered-chart-test.cpp layered-chart-test.h
					recursive-algorithm.cpp recursive-algorithm.h)



