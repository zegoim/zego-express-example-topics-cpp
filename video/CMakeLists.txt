cmake_minimum_required(VERSION 3.6)

project(ZegoExpressExampleVideo)

add_subdirectory(src)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ZegoExpressExampleVideo)
