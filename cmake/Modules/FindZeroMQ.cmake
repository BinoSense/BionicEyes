# - Find ZMQ
# Find the native zmq includes and library
#
#  ZEROMQ_INCLUDE_DIRS - where to find zmq.h, etc.
#  ZEROMQ_LIBRARIES   - List of libraries when using zmq.
#  ZEROMQ_FOUND       - True if zmq found.

set(ZEROMQ_INCLUDE_DIR "${DEPENDENCIES_PATH}/zeromq-4.3.2/include")

if(ARM)
	set(ZEROMQ_INCLUDE_DIR "${DEPENDENCIES_PATH}/zeromq-4.3.2/tegra/include")
	set(ZEROMQ_LIBRARY ${DEPENDENCIES_PATH}/zeromq-4.3.2/tegra/lib/libzmq.so)
elseif(UNIX)
	set(ZEROMQ_INCLUDE_DIR "${DEPENDENCIES_PATH}/zeromq-4.3.2/linux/include")
	set(ZEROMQ_LIBRARY ${DEPENDENCIES_PATH}/zeromq-4.3.2/linux/lib/libzmq.so)
endif(ARM)

MESSAGE(STATUS "    Find zeromq libraries: ${ZEROMQ_LIBRARY}")
MESSAGE(STATUS "    Find zeromq include path: ${ZEROMQ_INCLUDE_DIR}")
