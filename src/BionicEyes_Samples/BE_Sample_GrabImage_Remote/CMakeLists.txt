set(TARGET_NAME evo_be_Sample_GrabImage_Remote)
set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")

find_package(OpenCV_NoCUDA REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_PATH}/include)

add_executable(${TARGET_NAME_EXE}  BE_Sample_GrabImage_Remote.cpp)

target_link_libraries(
        ${TARGET_NAME_EXE}

        -Wl,--start-group
        ${OpenCV_LIBS}
        evo_be_BionicEyes_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}
        -Wl,--end-group
)
