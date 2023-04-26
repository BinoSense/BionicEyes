cmake_minimum_required(VERSION 3.1)

#build all CMakeList in subdirs

include(${PROJECT_PATH}/macros.cmake)

get_filename_component(FOLDERNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" FOLDERNAME ${FOLDERNAME})

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT ARM)
    foreach(subdir ${SUBDIRS})
        add_subdirectory(${subdir})
    endforeach()
endif(NOT ARM)