set(TARGET_NAME evo_be_Sample_JoyStick)
set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")

find_package(OpenCV_NoCUDA REQUIRED)

include_directories(${PROJECT_PATH}/include ${OpenCV_INCLUDE_DIRS})

add_executable(${TARGET_NAME_EXE}  BE_Sample_JoyStick.cpp)

target_link_libraries(
        ${TARGET_NAME_EXE}

        -Wl,--start-group
        ${OpenCV_LIBS}
        evo_be_BionicEyes_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}
        -Wl,--end-group
)
