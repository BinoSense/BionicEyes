cmake_minimum_required(VERSION 3.14)

PROJECT(BionicEyes)

set(${CMAKE_PROJECT_NAME}_SUMMARY              "SDK for Eyevolution Bionic Eye Products")
set(${CMAKE_PROJECT_NAME}_VENDOR_NAME          "Shanghai Eyevolution Technology Co.,Ltd")
set(${CMAKE_PROJECT_NAME}_VENDOR_SHORTNAME     "Eyevolution")
set(${CMAKE_PROJECT_NAME}_VENDOR_CONTACT       "wang.kaifang@ilooktech.com")
set(${CMAKE_PROJECT_NAME}_VENDOR_DOMAIN        "http://www.ilooktech.com/")
set(${CMAKE_PROJECT_NAME}_VERSION_MAJOR        1)
set(${CMAKE_PROJECT_NAME}_VERSION_MINOR        0)
set(${CMAKE_PROJECT_NAME}_VERSION_PATCH        0)
set(PROJECT_VERSION
  "${${CMAKE_PROJECT_NAME}_VERSION_MAJOR}.${${CMAKE_PROJECT_NAME}_VERSION_MINOR}.${${CMAKE_PROJECT_NAME}_VERSION_PATCH}")

set(PROJECT_PATH                               "${CMAKE_CURRENT_SOURCE_DIR}")
set(BE_SYSTEM_PATH                             "/usr/Evo_BionicEyes")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}     "${PROJECT_PATH}/cmake/Modules")
set(INCLUDE_PATH                               "${PROJECT_PATH}/include")
set(INCLUDE_PATH_BACKUP                        "${BE_SYSTEM_PATH}/include")
set(LIB_PATH                                   "${PROJECT_PATH}/lib")
set(LIB_PATH_BACKUP                            "${BE_SYSTEM_PATH}/lib")
set(INTERNAL_INCLUDE_PATH                      "${PROJECT_PATH}/include/internal")
set(INTERNAL_LIB_PATH                          "${PROJECT_PATH}/lib/internal")
set(INTERNAL_LIB_PATH_BACKUP                   "${BE_SYSTEM_PATH}/lib/internal")
set(DEPENDENCIES_PATH                          "${BE_SYSTEM_PATH}/dependencies")
set(PATCH_PATH                          	   "${PROJECT_PATH}/patch")
set(BIN_PATH                                   "${PROJECT_PATH}/bin")
set(SRC_PATH                                   "${PROJECT_PATH}/src")

execute_process(
        COMMAND cat "/etc/lsb-release"
        #        COMMAND grep -E "18.04"
        OUTPUT_VARIABLE UBUNTU_STR)

if(UBUNTU_STR MATCHES "16.04")
    message(STATUS "SYSTEM DISTRIB_RELEASE is 16.04")
    set(UBUNTU_DISTRIB_RELEASE "1604")
elseif(UBUNTU_STR MATCHES "18.04")
    message(STATUS "SYSTEM DISTRIB_RELEASE is 18.04")
    set(UBUNTU_DISTRIB_RELEASE "1804")
elseif(UBUNTU_STR MATCHES "20.04")
    message(STATUS "SYSTEM DISTRIB_RELEASE is 20.04")
    set(UBUNTU_DISTRIB_RELEASE "2004")
elseif(UBUNTU_STR MATCHES "22.04")
    message(STATUS "SYSTEM DISTRIB_RELEASE is 22.04")
    set(UBUNTU_DISTRIB_RELEASE "2204")
else()
    message(STATUS "Unsupported SYSTEM DISTRIB_RELEASE!!!")
    set(UBUNTU_DISTRIB_RELEASE "Unknown")
endif()

#.exe .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_PATH})
#.so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH_BACKUP})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_PATH_BACKUP})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_PATH_BACKUP})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LIB_PATH_BACKUP})
#.a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH_BACKUP})

option(BE_DEP_UPDATE                            "Update project dependencies"  OFF)
option(BE_LIB_UPDATE                            "Update project libs"  ON)
option(BE_INCLUDE_UPDATE                        "Update project include folder"  ON)
option(BE_COMPILE_V3                            "Compile BionicEyes device hardware version 3"  ON)
option(BE_COMPILE_V5                            "Compile BionicEyes device hardware version 5"  ON)
option(BUILD_SAMPLES                            "Build samples"                  ON)
option(BUILD_TEST                               "Build test"                  ON)
option(BUILD_3399Pro                            "Build 3399pro"                  OFF)
option(CV_4                                     "Use opencv4 or opencv3"        ON)

if (CV_4)
    add_definitions(-DCV_4)
endif()

# Allow the developer to select if Dynamic or Static libraries are built
option (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option (BUILD_STATIC_LIBS "Build Static Libraries" OFF)

# enable folders for each target
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall -s")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -s" )
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s" )

# default built type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "Compiling on Type " ${CMAKE_BUILD_TYPE})

include(cmake/Utils.cmake)
include(cmake/GetArch.cmake)
get_architecture(arch)
message(STATUS "get target arch (32/64 bit):" ${arch})

# work out the postfix; required where we use OUTPUT_NAME
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(EXE_POSTFIX ${CMAKE_RELEASE_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set(EXE_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    set(EXE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif(CMAKE_BUILD_TYPE MATCHES Release)

#set THIRDPARTY_LIB_ARCH_NAME as x64 or x86
set(DEPENDENCIES_ARCH_NAME "x64")
if(${arch} AND ${arch} STREQUAL "32")
    set(DEPENDENCIES_ARCH_NAME "x86")
endif()

if(BE_LIB_UPDATE MATCHES ON)
    message(STATUS "set to update libs")
else()
    message(STATUS "set not to update libs")
endif(BE_LIB_UPDATE MATCHES ON)

if(BE_INCLUDE_UPDATE MATCHES ON)
    message(STATUS "set to update include folder")
else()
    message(STATUS "set not to update include folder")
endif(BE_INCLUDE_UPDATE MATCHES ON)

if(BE_DEP_UPDATE MATCHES ON)
    message(STATUS "set to update dependencies")
else()
    message(STATUS "set not to update dependencies")
endif(BE_DEP_UPDATE MATCHES ON)

# Set platform-specific compiler flags.
# Check Embedded Platform
is_archarm(ARM)
if(WIN32)
    set(EVO_BE_ENV "EVO_BE_ENV_WINDOWS")
    set(PLATFORM_FOLDER "win/${DEPENDENCIES_ARCH_NAME}")
    set(PLATFORM_NAME "win-${DEPENDENCIES_ARCH_NAME}")
    message(STATUS "Compiling on Windows " ${CMAKE_BUILD_TYPE})

elseif(ARM)
    set(EVO_BE_ENV "EVO_BE_ENV_ARM")
    set(PLATFORM_FOLDER "tegra/arm${arch}")
    set(PLATFORM_NAME "tegra-arm${arch}")
    message(STATUS "Compiling on ARM " ${CMAKE_BUILD_TYPE})
    if(EXISTS "${BE_SYSTEM_PATH}/dependencies/")
        execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/resource/")
        execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/resource/" "${BE_SYSTEM_PATH}/resource/")
    else()
        execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/resource/" "${BE_SYSTEM_PATH}/resource/")
    endif(EXISTS "${BE_SYSTEM_PATH}/dependencies/")
    # renew Dependencies folder
    if(EXISTS "${BE_SYSTEM_PATH}/dependencies/")
        if(BE_DEP_UPDATE MATCHES ON)
            message(STATUS "Dependencies folder exists, updating...")
            execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/dependencies/")
            execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/dependencies/" "${BE_SYSTEM_PATH}/dependencies/")
        else()
            message(STATUS "Dependencies folder exists, don't excute copy.")
        endif(BE_DEP_UPDATE MATCHES ON)
    else()
        message(STATUS "Dependencies folder don't exists, copying......")
        execute_process(COMMAND cmake -E
                copy_directory "${PROJECT_PATH}/dependencies/" "${BE_SYSTEM_PATH}/dependencies/")
    endif(EXISTS "${BE_SYSTEM_PATH}/dependencies/")

    # renew lib folder
    if(EXISTS "${BE_SYSTEM_PATH}/lib/")
        if(BE_LIB_UPDATE MATCHES ON)
            message(STATUS "Bionic library folder exists, updating...")
            execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/lib/")
            execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/lib/" "${BE_SYSTEM_PATH}/lib/")
        else()
            message(STATUS "Bionic library folder exists, don't excute copy.")
        endif(BE_LIB_UPDATE MATCHES ON)
    else()
        message(STATUS "Bionic library folder don't exists, copying......")
        execute_process(COMMAND cmake -E
                copy_directory "${PROJECT_PATH}/lib/" "${BE_SYSTEM_PATH}/lib/")
    endif(EXISTS "${BE_SYSTEM_PATH}/lib/")

    # renew include folder
    if(EXISTS "${BE_SYSTEM_PATH}/include/")
        if(BE_INCLUDE_UPDATE MATCHES ON)
            message(STATUS "Bionic include folder exists, updating...")
            execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/include/")
            execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/include/" "${BE_SYSTEM_PATH}/include/")
        else()
            message(STATUS "Bionic include folder exists, don't excute copy.")
        endif(BE_INCLUDE_UPDATE MATCHES ON)
    else()
        message(STATUS "Bionic include folder don't exists, copying......")
        execute_process(COMMAND cmake -E
                copy_directory "${PROJECT_PATH}/include/" "${BE_SYSTEM_PATH}/include/")
    endif(EXISTS "${BE_SYSTEM_PATH}/include/")

elseif(UNIX)
    set(EVO_BE_ENV "EVO_BE_ENV_LINUX")
    set(PLATFORM_FOLDER "linux/${DEPENDENCIES_ARCH_NAME}")
    set(PLATFORM_NAME "linux-${DEPENDENCIES_ARCH_NAME}")
    message(STATUS "Compiling on Unix " ${CMAKE_BUILD_TYPE})
    if(EXISTS "${BE_SYSTEM_PATH}/dependencies/")
        execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/resource/")
        execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/resource/" "${BE_SYSTEM_PATH}/resource/")
    else()
        execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/resource/" "${BE_SYSTEM_PATH}/resource/")
    endif(EXISTS "${BE_SYSTEM_PATH}/dependencies/")
    # renew Dependencies folder
    if(EXISTS "${BE_SYSTEM_PATH}/dependencies/")
        if(BE_DEP_UPDATE MATCHES ON)
            message(STATUS "Dependencies folder exists, updating...")
            execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/dependencies/")
            execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/dependencies/" "${BE_SYSTEM_PATH}/dependencies/")
        else()
            message(STATUS "Dependencies folder exists, don't excute copy.")
        endif(BE_DEP_UPDATE MATCHES ON)
    else()
        message(STATUS "Dependencies folder don't exists, copying......")
        execute_process(COMMAND cmake -E
            copy_directory "${PROJECT_PATH}/dependencies/" "${BE_SYSTEM_PATH}/dependencies/")
    endif(EXISTS "${BE_SYSTEM_PATH}/dependencies/")

    # renew lib folder
    if(EXISTS "${BE_SYSTEM_PATH}/lib/")
        if(BE_LIB_UPDATE MATCHES ON)
            message(STATUS "Bionic library folder exists, updating...")
            execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/lib/")
            execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/lib/" "${BE_SYSTEM_PATH}/lib/")
        else()
            message(STATUS "Bionic library folder exists, don't excute copy.")
        endif(BE_LIB_UPDATE MATCHES ON)
    else()
        message(STATUS "Bionic library folder don't exists, copying......")
        execute_process(COMMAND cmake -E
                copy_directory "${PROJECT_PATH}/lib/" "${BE_SYSTEM_PATH}/lib/")
    endif(EXISTS "${BE_SYSTEM_PATH}/lib/")

    # renew include folder
    if(EXISTS "${BE_SYSTEM_PATH}/include/")
        if(BE_INCLUDE_UPDATE MATCHES ON)
            message(STATUS "Bionic include folder exists, updating...")
            execute_process(COMMAND rm -r "${BE_SYSTEM_PATH}/include/")
            execute_process(COMMAND cmake -E copy_directory "${PROJECT_PATH}/include/" "${BE_SYSTEM_PATH}/include/")
        else()
            message(STATUS "Bionic include folder exists, don't excute copy.")
        endif(BE_INCLUDE_UPDATE MATCHES ON)
    else()
        message(STATUS "Bionic include folder don't exists, copying......")
        execute_process(COMMAND cmake -E
                copy_directory "${PROJECT_PATH}/include/" "${BE_SYSTEM_PATH}/include/")
    endif(EXISTS "${BE_SYSTEM_PATH}/include/")

endif(WIN32)

if(NOT EXISTS "${BE_SYSTEM_PATH}/io_path/")
    message(STATUS "Bionic IO folder don't exists, creating......")
    execute_process(COMMAND cmake -E
            make_directory "${BE_SYSTEM_PATH}/io_path/")
endif(NOT EXISTS "${BE_SYSTEM_PATH}/io_path/")

if(NOT EXISTS "${BE_SYSTEM_PATH}/log_path/")
    message(STATUS "Bionic LOG folder don't exists, creating......")
    execute_process(COMMAND cmake -E
            make_directory "${BE_SYSTEM_PATH}/log_path/")
endif(NOT EXISTS "${BE_SYSTEM_PATH}/log_path/")

if(NOT EXISTS "${BE_SYSTEM_PATH}/tmp_path")
    message(STATUS "Bionic TMP folder don't exists, creating......")
    execute_process(COMMAND cmake -E
            make_directory "${BE_SYSTEM_PATH}/tmp_path")
endif(NOT EXISTS "${BE_SYSTEM_PATH}/tmp_path")

include_directories(${INCLUDE_PATH} ${INTERNAL_INCLUDE_PATH})
link_directories(${LIB_PATH_BACKUP} ${INTERNAL_LIB_PATH_BACKUP} ${BIN_PATH})

set(CMAKE_CXX_STANDARD 11) #C++ 11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #..without compiler extensions like gnu++11

include(macros.cmake)

file(RELATIVE_PATH REL_INCLUDE_DIR "${PROJECT_PATH}" "${INCLUDE_PATH}")
file(RELATIVE_PATH REL_LIB_DIR "${PROJECT_PATH}" "${LIB_PATH}")
file(RELATIVE_PATH REL_BIN_DIR "${PROJECT_PATH}" "${BIN_PATH}")
file(RELATIVE_PATH REL_BIN_DIR "${PROJECT_PATH}" "${BIN_PATH}")
file(RELATIVE_PATH REL_PATCH_DIR "${PROJECT_PATH}" "${PATCH_PATH}")
#file(RELATIVE_PATH REL_DOC_DIR "${PROJECT_BINARY_DIR}" "${DOXYGEN_PATH}")

# Use SSE
if (USE_SSE)
    message(STATUS "Use SSE")
    if(WIN32) # Windows
        add_definitions(-D__SSE__)
        add_definitions(-D__SSE2__)
        add_definitions(-D__SSE4_1__)
        add_definitions(-DPNG_INTEL_SSE)
    elseif(ARM AND ${arch} STREQUAL "32" )#TK1
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")
        add_definitions(-DPNG_ARM_NEON_CHECK_SUPPORTED)
    elseif(ARM)#TX1
        add_definitions(-DPNG_ARM_NEON_CHECK_SUPPORTED)
    elseif(UNIX) #Other Unix
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
    endif(WIN32)
endif(USE_SSE)

add_subdirectory(src)


