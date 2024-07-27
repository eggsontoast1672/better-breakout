cmake_minimum_required(VERSION 3.5)
project(better-breakout)

find_package(SDL2 REQUIRED)
find_package(sdl2_image REQUIRED)

add_executable(breakout src/glad.c src/main.cpp)
target_include_directories(breakout PRIVATE include)
target_link_libraries(breakout PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)
