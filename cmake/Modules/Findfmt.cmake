set(FMT_INCLUDE_DIR ${DEPENDENCIES_PATH}/fmt/fmt/include)

if(WIN32)
    set(FMT_LIBRARY ${DEPENDENCIES_PATH}/fmt/fmt/lib/libfmt.a)
elseif(UNIX)
    set(FMT_LIBRARY ${DEPENDENCIES_PATH}/fmt/fmt/lib/libfmt.a)
endif(WIN32)

MESSAGE(STATUS "    Find fmt include: ${FMT_INCLUDE_DIR}")
MESSAGE(STATUS "    Find fmt libraries: ${FMT_LIBRARY}")