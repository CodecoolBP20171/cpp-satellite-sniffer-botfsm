cmake_minimum_required(VERSION 3.16)

set(IMGUI_SRC
        imgui.cpp
        imgui_draw.cpp
        imgui_demo.cpp
        imgui_impl_opengl3.cpp
        imgui_impl_sdl.cpp
        imgui_widgets.cpp
        )

add_library(imgui ${IMGUI_SRC})

target_include_directories(imgui PRIVATE
        ${SDL2_INCLUDE_DIRS}
        )
