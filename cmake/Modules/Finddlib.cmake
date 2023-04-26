## - Find Dlib
## Find the native dlib includes and library
##
##  DLIB_INCLUDE_DIRS - where to find dlib/algs.h, etc.
##  DLIB_LIBRARIES   - List of libraries when using zlib.
##  DLIB_FOUND       - True if dlib found.
#
#find_path(DLIB_INCLUDE_DIR dlib/algs.h
#	PATHS
#	${DEPENDENCIES_PATH}/dlib-static-19.13/${PLATFORM_FOLDER}/include
#	)
#
#find_library(DLIB_LIBRARY NAMES dlib
#    PATHS
#    ${DEPENDENCIES_PATH}/dlib-static-19.13/${PLATFORM_FOLDER}/lib
#	NO_SYSTEM_ENVIRONMENT_PATH
#    )
#MESSAGE(STATUS "    Find dlib libraries: ${DLIB_LIBRARY}")
#MESSAGE(STATUS "    Find dlib include path: ${DLIB_INCLUDE_DIR}")
#
#set (DLIB_INCLUDE_DIRS ${DLIB_INCLUDE_DIR})
#set (DLIB_LIBRARIES ${DLIB_LIBRARY})
#
#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(dlib
#                                  REQUIRED_VARS DLIB_INCLUDE_DIR DLIB_LIBRARY)
#
#MARK_AS_ADVANCED (DLIB_INCLUDE_DIR DLIB_LIBRARY)
#
#MESSAGE(STATUS "    Find Dlib include: ${DLIB_INCLUDE_DIR}")
#MESSAGE(STATUS "    Find Dlib libraries: ${DLIB_LIBRARY}")



set(DLIB_INCLUDE_DIR ${DEPENDENCIES_PATH}/dlib-static-19.13/${PLATFORM_FOLDER}/include)

if(WIN32)
	set(DLIB_LIBRARY ${DEPENDENCIES_PATH}/dlib-static-19.13/linux/x64/lib/libdlib.a)
elseif(UNIX)
	set(DLIB_LIBRARY ${DEPENDENCIES_PATH}/dlib-static-19.13/linux/x64/lib/libdlib.a)
endif(WIN32)

MESSAGE(STATUS "    Find Dlib include: ${DLIB_INCLUDE_DIR}")
MESSAGE(STATUS "    Find Dlib libraries: ${DLIB_LIBRARY}")