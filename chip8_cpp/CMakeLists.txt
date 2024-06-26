cmake_minimum_required(VERSION 3.20.0)
project(chip8emulator)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SourceFiles
        src/main.cpp
        src/chip8.cpp
        src/renderer.cpp
        src/cpu.cpp
        src/memory.cpp
        src/sound.cpp
        include/sound.h
        include/memory.h
        include/cpu.h
        include/renderer.h
        include/chip8.h
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SourceFiles})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_mixer::SDL2_mixer)