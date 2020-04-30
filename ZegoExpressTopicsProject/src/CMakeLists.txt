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

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_LIST_DIR})

if(WIN32)

    set(exe_property WIN32)

    include_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/win/include)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/win/lib/${WINDOWS_PLATFORM})

    add_executable(${PROJECT_NAME} ${exe_property} ${All_SOURCES})
    target_link_libraries(${PROJECT_NAME} ZegoExpressEngine.lib)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    add_definitions(-DUNICODE -D_UNICODE -D_SCL_SECURE_NO_WARNINGS -DWIN32)

endif(WIN32)

if(APPLE)

    SET(BUILD_WITH_INSTALL_RPATH TRUE)
    set(exe_property MACOSX_BUNDLE)

    include_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/mac/include)
    link_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/mac/lib/)

    add_executable(${PROJECT_NAME} ${exe_property} ${All_SOURCES})
    target_link_libraries(${PROJECT_NAME} libZegoExpressEngine.dylib)

    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_GCC_INPUT_FILETYPE "sourcecode.cpp.objcpp")
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES")
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_SEPARATE_STRIP "YES")
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_STRIP_STYLE "Non-Global Symbols")
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/Info.plist.in)

endif(APPLE)


qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Widgets)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/../bin)

if(WIN32)
    set(ZegoExpressEnginedll ${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/win/lib/${WINDOWS_PLATFORM}/ZegoExpressEngine.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy ${ZegoExpressEnginedll} ${EXECUTABLE_OUTPUT_PATH}/$(CONFIGURATION)
    )
endif(WIN32)

if(APPLE)
    set(ZegoExpressEnginedll ${CMAKE_CURRENT_LIST_DIR}/../libs/ZegoExpress/mac/lib/libZegoExpressEngine.dylib)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
            COMMAND mkdir ${EXECUTABLE_OUTPUT_PATH}/$(CONFIGURATION)/${PROJECT_NAME}.app/Contents/Frameworks
            COMMAND cp ${ZegoExpressEnginedll} ${EXECUTABLE_OUTPUT_PATH}/$(CONFIGURATION)/${PROJECT_NAME}.app/Contents/Frameworks
    )
endif(APPLE)





