function(copy_runtime_libraries target)
	add_custom_command(
		TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
		COMMAND_EXPAND_LISTS)
endfunction()

# This function assumes that resource_dir is simply the bare name, not the full
# path. This enables it to copy the directory contents to the correct location.
function(copy_runtime_resources target resource_dir)
	add_custom_command(
		TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${resource_dir} $<TARGET_FILE_DIR:${target}>/${resource_dir})
endfunction()

cmake_minimum_required(VERSION 3.21)
project(better-breakout)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

find_package(SDL2 REQUIRED)
find_package(sdl2_image REQUIRED)

get_target_property(SDL2_INCLUDE_DIRS SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SDL2_IMAGE_INCLUDE_DIRS SDL2_image::SDL2_image INTERFACE_INCLUDE_DIRECTORIES)

add_executable(breakout src/glad.c src/main.cpp)
target_include_directories(breakout PRIVATE include ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(breakout PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)

if(WIN32)
	target_link_libraries(breakout PRIVATE SDL2::SDL2main)

	copy_runtime_libraries(breakout)
	copy_runtime_resources(breakout res)
endif()