#if(WIN32) # Windows
set(JPEG_INCLUDE_DIR ${DEPENDENCIES_PATH}/libjpeg-turbo-2.0.2/include/)
set(JPEG_LIBRARY ${DEPENDENCIES_PATH}/libjpeg-turbo-2.0.2/${DEPENDENCIES_ARCH_NAME}/libturbojpeg.so)
#find_path(JPEG_INCLUDE_DIR NAMES turbojpeg.h
#		PATHS
#		${DEPENDENCIES_PATH}/libjpeg-turbo-2.0.2/include/
#		NO_SYSTEM_ENVIRONMENT_PATH
#
#	)
#find_library(JPEG_LIBRARY NAMES libturbojpeg.so
#		PATHS
#		${DEPENDENCIES_PATH}/libjpeg-turbo-2.0.2/${DEPENDENCIES_ARCH_NAME}/
#		NO_SYSTEM_ENVIRONMENT_PATH
#	)

MESSAGE(STATUS "    Find TurboJpeg libraries: ${JPEG_LIBRARY}")
MESSAGE(STATUS "    Find TurboJpeg include path: ${JPEG_INCLUDE_DIR}")

#else()
#find_path(JPEG_INCLUDE_DIR turbojpeg.h)
#find_library(JPEG_LIBRARY NAMES turbojpeg)
#endif(WIN32)

# handle the QUIETLY and REQUIRED arguments and set JPEG_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JPEG DEFAULT_MSG JPEG_LIBRARY JPEG_INCLUDE_DIR)

if(JPEG_FOUND)
  set(JPEG_LIBRARIES ${JPEG_LIBRARY})
endif()

# Deprecated declarations.
set (NATIVE_JPEG_INCLUDE_PATH ${JPEG_INCLUDE_DIR} )
if(JPEG_LIBRARY)
  get_filename_component (NATIVE_JPEG_LIB_PATH ${JPEG_LIBRARY} PATH)
endif()

mark_as_advanced(JPEG_LIBRARY JPEG_INCLUDE_DIR )
