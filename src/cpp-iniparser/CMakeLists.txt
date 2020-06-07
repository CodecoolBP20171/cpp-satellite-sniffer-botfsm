cmake_minimum_required(VERSION 3.16)

set(INI_SRC 
  ${CMAKE_CURRENT_LIST_DIR}/src/INIConfig.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/INIParser.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Utilities.cpp
)

add_library(iniparser SHARED ${INI_SRC})
target_include_directories(iniparser PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)