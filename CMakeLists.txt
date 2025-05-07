cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(PikumaPhys VERSION 1)

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2IMAGE REQUIRED CONFIG REQUIRED COMPONENTS SDL2IMAGE)
find_package(SDL2GFX REQUIRED CONFIG REQUIRED COMPONENTS SDL2GFX)

find_package(GLEW REQUIRED)

add_compile_options (-fdiagnostics-color=always)
add_compile_options(-Wextra -Wall -Wpedantic -std=c++20)


# vim:shiftwidth=2:
