if(UNIX)
    set(TARGET_NAME evo_be_Sample_ThreeAxis)
    set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")

    include_directories(${PROJECT_PATH}/include)

    find_package(OpenCV_NoCUDA REQUIRED)

    include_directories(${OpenCV_INCLUDE_DIRS})

    add_executable(${TARGET_NAME_EXE}  BE_Sample_ThreeAxis.cpp)

    target_link_libraries(
            ${TARGET_NAME_EXE}
            -Wl,--start-group
            ${OpenCV_LIBS}
            evo_be_Internal_shared_${UBUNTU_DISTRIB_RELEASE}
            evo_be_Device_ThreeAxis_shared_${UBUNTU_DISTRIB_RELEASE}
            pthread
            evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}
            X11
            -Wl,--end-group

    )
endif(UNIX)
