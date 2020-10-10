cmake_minimum_required(VERSION 3.16)

set(SGP_SRC
        CoordGeodetic.cpp
        CoordTopocentric.cpp
        DateTime.cpp
        DecayedException.cpp
        Eci.cpp
        Globals.cpp
        Observer.cpp
        OrbitalElements.cpp
        SGP4.cpp
        SatelliteException.cpp
        SolarPosition.cpp
        TimeSpan.cpp
        Tle.cpp
        TleException.cpp
        Util.cpp
        Vector.cpp
        )

add_library(sgp4 ${SGP_SRC})
