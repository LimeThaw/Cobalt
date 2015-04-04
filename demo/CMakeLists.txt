include_directories(${PROJECT_SOURCE_DIR}/engine)
add_executable(CobaltDemo src/main.cpp)
target_link_libraries(CobaltDemo Cobalt)
target_link_libraries(CobaltDemo "-lSOIL")