cmake_minimum_required(VERSION 2.8)
project(Cobalt C CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(SOIL_INCLUDE_DIRS "/usr/include" CACHE PATH "SOIL Include Path")
set(SOIL_LIB_DIR "/usr/lib" CACHE PATH "SOIL Lib Path")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
link_directories(${SOIL_LIB_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

add_subdirectory(engine/)
add_subdirectory(demo/)
