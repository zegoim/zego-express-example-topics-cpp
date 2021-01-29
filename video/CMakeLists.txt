cmake_minimum_required(VERSION 3.6)

project(ZegoExpressExampleVideo)

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

macro(ADD_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${CMAKE_OSX_SYSROOT}/System/Library
        PATH_SUFFIXES Frameworks
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} ${FRAMEWORK_${fwname}})
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_FRAMEWORK)

add_subdirectory(src)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ZegoExpressExampleVideo)
