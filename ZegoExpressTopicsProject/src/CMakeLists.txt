cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME ZegoExpressTopics)

project(${PROJECT_NAME})

if(WINDOWS_PLATFORM STREQUAL "x86")
    set(CMAKE_PREFIX_PATH $ENV{QT_DIR})
elseif(WINDOWS_PLATFORM STREQUAL "x64")
    set(CMAKE_PREFIX_PATH $ENV{QT_DIR_x64})
endif()

# https://cmake.org/cmake/help/v3.6/manual/cmake-qt.7.html
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)

set(AUTORCC ON)

file(GLOB_RECURSE All_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.mm 
                              ${CMAKE_CURRENT_LIST_DIR}/*.m 
                              ${CMAKE_CURRENT_LIST_DIR}/*.cpp 
                              ${CMAKE_CURRENT_LIST_DIR}/*.mm 
                              ${CMAKE_CURRENT_LIST_DIR}/*.c 
                              ${CMAKE_CURRENT_LIST_DIR}/*.cc 
                              ${CMAKE_CURRENT_LIST_DIR}/*.hh 
                              ${CMAKE_CURRENT_LIST_DIR}/*.h
                              ${CMAKE_CURRENT_LIST_DIR}/*.ui
                              ${CMAKE_CURRENT_LIST_DIR}/*.rc)

macro(source_group_by_dir source_files)       
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR}/)
    foreach(sgbd_file ${${source_files}})
        string(REGEX REPLACE ${sgbd_cur_dir} "" sgbd_fpath ${sgbd_file})
        string(REGEX REPLACE "[^/\\\\]+$" "" sgbd_group_name ${sgbd_fpath})
        if(sgbd_group_name)
            string(REPLACE "/"   "\\\\"  sgbd_group_name_normal ${sgbd_group_name})
            source_group(${sgbd_group_name_normal} FILES ${sgbd_file})
        endif(sgbd_group_name)	
    endforeach(sgbd_file)
endmacro(source_group_by_dir)

source_group_by_dir(All_SOURCES)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/include
                    ${CMAKE_CURRENT_LIST_DIR})
link_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/lib/${WINDOWS_PLATFORM})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
add_definitions(-DUNICODE -D_UNICODE -D_SCL_SECURE_NO_WARNINGS -DWIN32 -DV_LOG -DCURL_STATICLIB)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/../bin)

add_executable(${PROJECT_NAME} WIN32 ${All_SOURCES})

target_link_libraries(${PROJECT_NAME} ZegoExpressEngine.lib)

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Widgets)

set(ZegoExpressEnginedll ${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/lib/${WINDOWS_PLATFORM}/ZegoExpressEngine.dll)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy ${ZegoExpressEnginedll} ${CMAKE_CURRENT_LIST_DIR}/../bin/$(CONFIGURATION)
                   )




