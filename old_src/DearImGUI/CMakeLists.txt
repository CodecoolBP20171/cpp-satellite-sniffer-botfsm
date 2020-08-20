cmake_minimum_required(VERSION 3.16)

set(IMGUI_SRC
  ${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui_demo.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_sdl.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
  )

add_library(imgui SHARED ${IMGUI_SRC})
target_include_directories(imgui PRIVATE ${SDL2_INCLUDE_DIRS})
