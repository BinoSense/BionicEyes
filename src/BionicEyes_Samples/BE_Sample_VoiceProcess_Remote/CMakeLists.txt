set(TARGET_NAME evo_be_Sample_VoiceProcess_Remote)
set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")

include_directories(${PROJECT_PATH}/include)

find_package(OpenCV_NoCUDA REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${TARGET_NAME_EXE}  BE_Sample_VoiceProcess_Remote.cpp)

target_link_libraries(
        ${TARGET_NAME_EXE}
        -Wl,--start-group
        evo_be_BionicEyes_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}
        evo_be_Voice_shared_${UBUNTU_DISTRIB_RELEASE}
        ${OpenCV_LIBS}
        -Wl,--end-group
)
