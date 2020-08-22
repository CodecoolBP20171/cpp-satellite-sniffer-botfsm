cmake_minimum_required(VERSION 3.16)

project(Sniffer)

set_property(GLOBAL PROPERTY CXX_STANDARD 17)
# need this because cmake decided to be an a$$ and doesn't use -g or define _GLIBCXX_DEBUG for clang
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

add_subdirectory(old_src)
# add_subdirectory(src)