set(TARGET_NAME evo_be_Sample_StereoRectify)
set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")

find_package(OpenCV_CUDA REQUIRED)
find_package(fmt REQUIRED)

find_package (Threads REQUIRED)
find_package(Eigen3 REQUIRED)
add_definitions     ( ${EIGEN3_DEFINITIONS} )
include_directories ( ${EIGEN3_INCLUDE_DIRS} )

include_directories(
        /usr/local/cuda/include
        ${OpenCV_INCLUDE_DIRS}
        ${FMT_INCLUDE_DIR}
        ${DEPENDENCIES_PATH}         
)

add_executable(  ${TARGET_NAME_EXE}  BE_Sample_StereoRectify.cpp )


target_link_libraries( ${TARGET_NAME_EXE}
        -Wl,--start-group        
        evo_be_BionicEyes_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Internal_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}        
        evo_be_ImageProcess_Core_CUDA_shared_${UBUNTU_DISTRIB_RELEASE}                       
        ${CUDA_LIBRARIES}
        ${OpenCV_LIBS}        
        ${CMAKE_THREAD_LIBS_INIT}
        -Wl,--end-group
)


