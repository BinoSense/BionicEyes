set(TARGET_NAME evo_be_Sample_Gaze_Parallel)
set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")
#set(CMAKE_C_COMPILER "/usr/bin/g++-9")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++-9")

add_definitions(-DBUILD_WITH_OPENCV)
set(CMAKE_CXX_FLAGS "-msse4 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fsee -fomit-frame-pointer  -funroll-loops -Wno-sign-compare")
find_package(Eigen3 REQUIRED)
find_package(OpenCV_CUDA REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} )
include_directories(${PROJECT_PATH}/include ${TINYCGL_PUBLIC_INCLUDE_DIRECTORIES})


add_executable(${TARGET_NAME_EXE} BE_Sample_Gaze_Parallel.cpp  )

target_link_libraries(
        ${TARGET_NAME_EXE}
        -Wl,--start-group
        ${OpenCV_LIBS}
        ${CUDA_LIBRARIES}
        evo_be_BionicEyes_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Internal_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Device_Service_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_ImageProcess_Core_shared_${UBUNTU_DISTRIB_RELEASE}
        #evo_be_ImageProcess_Core_CUDA_shared_${UBUNTU_DISTRIB_RELEASE}
        -Wl,--end-group
)
