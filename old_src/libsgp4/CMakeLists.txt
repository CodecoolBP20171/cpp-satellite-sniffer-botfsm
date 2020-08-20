cmake_minimum_required(VERSION 3.16)

set(SGP_SRC
  ${CMAKE_CURRENT_LIST_DIR}/src/CoordGeodetic.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/CoordTopocentric.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/DateTime.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/DecayedException.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Eci.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Globals.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Observer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/OrbitalElements.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/SGP4.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/SatelliteException.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/SolarPosition.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/TimeSpan.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Tle.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/TleException.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Util.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Vector.cpp
  )

add_library(sgp4 SHARED ${SGP_SRC})
target_include_directories(sgp4 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)