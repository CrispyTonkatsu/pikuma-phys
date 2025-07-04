cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(PikumaPhys VERSION 1)

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include (FindPkgConfig)
include (FindSDL_image)
include (FindSDL_gfx)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
pkg_search_module(SDL_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL_GFX REQUIRED SDL2_gfx)

add_compile_options (-fdiagnostics-color=always)
add_compile_options(-Wextra -Wall -Wpedantic)

add_executable(engine 
./src/Main.cpp 
./src/Graphics.cpp
./src/Application.cpp
./src/Physics/Vec2.cpp
./src/Physics/Body.cpp
./src/Physics/Force.cpp
./src/Physics/Shape.cpp
./src/Physics/Collision.cpp
./src/Physics/Contact.cpp
./src/Physics/World.cpp
)

target_link_libraries(engine OpenGL SDL2 SDL2_image SDL2_gfx)
include_directories(engine ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

# vim:shiftwidth=2:
