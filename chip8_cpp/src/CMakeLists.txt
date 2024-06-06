include_directories(${chip8emulator_SOURCE_DIR}/include)

set(SourceFiles
    main.cpp
    chip8.cpp
    renderer.cpp
    memory.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SourceFiles})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})