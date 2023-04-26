if(WIN32)
    set(OpenCV_INCLUDE_DIRS "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include")
    set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include/opencv")
    set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include/opencv2")
    set(OpenCV_LIBS ${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/lib/opencv_world3411.lib)
elseif(ARM)
    if (CV_4)
        set(OpenCV_INCLUDE_DIRS "${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/include")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/include/opencv4/opencv2")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/include/opencv4")
        set(OpenCV_LIBS ${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/lib/libopencv_world.so)
    else()
        set(OpenCV_INCLUDE_DIRS "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/include")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/include/opencv")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/include/opencv2")
        set(OpenCV_LIBS ${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/lib/libopencv_world.so)
    endif ()
    #MESSAGE(STATUS "    Find opencv include: ${OpenCV_INCLUDE_DIRS}")
    #MESSAGE(STATUS "    Find opencv libraries: ${OpenCV_LIBS}")
elseif(UNIX)
    if (CV_4)
        set(OpenCV_INCLUDE_DIRS "${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include/opencv4/opencv2")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include/opencv4")
        set(OpenCV_LIBS ${DEPENDENCIES_PATH}/opencv-4.7.0/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/lib/libopencv_world.so)
        MESSAGE(STATUS "    Find opencv include: ${OpenCV_INCLUDE_DIRS}")
        #MESSAGE(STATUS "    Find opencv libraries: ${OpenCV_LIBS}")
    else()
        set(OpenCV_INCLUDE_DIRS "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include/opencv")
        set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} "${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/include/opencv2")
        set(OpenCV_LIBS ${DEPENDENCIES_PATH}/opencv-3.4.11/${PLATFORM_FOLDER}/${UBUNTU_DISTRIB_RELEASE}/WithoutCUDA/lib/libopencv_world.so)
        #MESSAGE(STATUS "    Find opencv include: ${OpenCV_INCLUDE_DIRS}")
        #MESSAGE(STATUS "    Find opencv libraries: ${OpenCV_LIBS}")
    endif()
    #MESSAGE(STATUS "    Find opencv include: ${OpenCV_INCLUDE_DIRS}")
    #MESSAGE(STATUS "    Find opencv libraries: ${OpenCV_LIBS}")
endif(WIN32)