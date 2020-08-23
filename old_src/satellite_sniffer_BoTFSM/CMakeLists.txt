cmake_minimum_required(VERSION 3.16)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

set(SNIFFER_SRC
  ${SRC_DIR}/Config.cpp
  ${SRC_DIR}/Map.cpp
  ${SRC_DIR}/Menu.cpp
  ${SRC_DIR}/Popup.cpp
  ${SRC_DIR}/Program.cpp
  ${SRC_DIR}/Resources.cpp
  ${SRC_DIR}/Satellite.cpp
  ${SRC_DIR}/Satellites.cpp
  ${SRC_DIR}/Trajectory.cpp
  ${SRC_DIR}/UIElement.cpp
  ${SRC_DIR}/UISatellite.cpp
  ${SRC_DIR}/main.cpp
)

add_executable(sniffer ${SNIFFER_SRC})
target_include_directories(sniffer PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${GLEW_INCLDUE_DIRS}
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/../cpp-iniparser/include
  ${CMAKE_CURRENT_LIST_DIR}/../libsgp4/include
  ${CMAKE_CURRENT_LIST_DIR}/../DearImGUI
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
  $<TARGET_LINKER_FILE:iniparser>
  $<TARGET_LINKER_FILE:imgui>
)
