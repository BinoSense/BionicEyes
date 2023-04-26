# - Find ZMQ
# Find the native zmq includes and library
#
#  ZEROMQ_INCLUDE_DIRS - where to find zmq.h, etc.
#  ZEROMQ_LIBRARIES   - List of libraries when using zmq.
#  ZEROMQ_FOUND       - True if zmq found.

set(Pangolin_INCLUDE_DIR "${DEPENDENCIES_PATH}/Pangolin-0.5/include;/usr/include")
set(Pangolin_INCLUDE_DIRS "${DEPENDENCIES_PATH}/Pangolin-0.5/include;/usr/include")

if(WIN32)
    set(Pangolin_LIBRARIES ${DEPENDENCIES_PATH}/Pangolin-0.5/lib/libpangolin.lib)
elseif(UNIX)
    set(Pangolin_LIBRARIES "${DEPENDENCIES_PATH}/Pangolin-0.5/lib/libpangolin.so")
endif(WIN32)

MESSAGE(STATUS "    Find Pangolin libraries: ${Pangolin_LIBRARIES}")
MESSAGE(STATUS "    Find Pangolin include path: ${Pangolin_INCLUDE_DIRS}")
