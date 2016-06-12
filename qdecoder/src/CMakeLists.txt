# libqdecoder
# Copyright Robin.Rowe@cinepaint.org 2015/7/19
# License MIT or BSD

cmake_minimum_required(VERSION 2.8)
project(CMake_qdecoder)

#version number
set(CMake_qdecoder_VERSION_MAJOR 1)
set(CMake_qdecoder_VERSION_MINOR 0)
set(CMake_qdecoder_VERSION_PATCH 0)

file(GLOB qdecoder_SRC
    "*.h"
    "*.cpp"
)

add_library( qdecoder STATIC ${qdecoder_SRC} )

