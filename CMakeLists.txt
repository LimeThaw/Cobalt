cmake_minimum_required(VERSION 3.1)
project(Cobalt C CXX)
set(SOIL_INCLUDE_DIRS "/usr/include" CACHE PATH "SOIL Include Path")
set(SOIL_LIB_DIR "/usr/lib" CACHE PATH "SOIL Lib Path")
link_directories(${SOIL_LIB_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

add_subdirectory(engine/)
add_subdirectory(demo/)
