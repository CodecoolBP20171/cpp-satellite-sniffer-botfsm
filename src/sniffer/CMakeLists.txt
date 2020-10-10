cmake_minimum_required(VERSION 3.16)

set(SNIFFER_SRC
        Config.cpp
        Map.cpp
        Menu.cpp
        Popup.cpp
        Program.cpp
        Resources.cpp
        Satellite.cpp
        Satellites.cpp
        Trajectory.cpp
        UIElement.cpp
        UISatellite.cpp
        main.cpp
        )

add_executable(sniffer ${SNIFFER_SRC})
target_include_directories(sniffer PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${GLEW_INCLDUE_DIRS}
        ${CMAKE_CURRENT_LIST_DIR}/../sgp4
        ${CMAKE_CURRENT_LIST_DIR}/../imgui
        ${CMAKE_CURRENT_LIST_DIR}/../../lib/rapidjson/include
        )

target_link_libraries(sniffer PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_GFX_LIBRARIES}
        $<TARGET_LINKER_FILE:sgp4>
        $<TARGET_LINKER_FILE:imgui>
        )

add_dependencies(sniffer sgp4 imgui)
