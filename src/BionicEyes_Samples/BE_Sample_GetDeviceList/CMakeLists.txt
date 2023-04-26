set(TARGET_NAME evo_be_Sample_GetDeviceList)
set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")

find_package(OpenCV_NoCUDA REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_PATH}/include)

add_executable(${TARGET_NAME_EXE}  BE_Sample_GetDeviceList.cpp)

target_link_libraries(
        ${TARGET_NAME_EXE}

        -Wl,--start-group
        ${OpenCV_LIBS}
        pthread
        evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}
        -Wl,--end-group
)
