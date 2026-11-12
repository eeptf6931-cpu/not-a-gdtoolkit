cmake_minimum_required(VERSION 3.21)
project(GDToolkit VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

add_geode_mod(GDToolkit
    SOURCES
        src/main.cpp
)
