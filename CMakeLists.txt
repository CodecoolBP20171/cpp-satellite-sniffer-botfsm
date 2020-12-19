cmake_minimum_required(VERSION 3.16)

project(Sniffer)

set_property(GLOBAL PROPERTY CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
set(SPDLOG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/spdlog/include CACHE PATH "spdlog library include directory")
set(RAPIDJOSN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/rapidjson/include CACHE PATH "rapidjson library include directory")

add_subdirectory(src)