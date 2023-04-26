set(TARGET_NAME evo_be_Sample_Log)
set(TARGET_NAME2 evo_be_Sample_Spdlog)
set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")
set(TARGET_NAME_EXE2 "${TARGET_NAME2}_${UBUNTU_DISTRIB_RELEASE}")

find_package(OpenCV_NoCUDA REQUIRED)
find_package(Spdlog REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${SPDLOG_INCLUDE_DIR})

include_directories(${PROJECT_PATH}/include .)

add_executable(${TARGET_NAME_EXE}  BE_Sample_Log.cpp BE_Sample_Log_Test.cpp)

add_executable(${TARGET_NAME_EXE2}  BE_Sample_spdlog.cpp)

target_link_libraries(
        ${TARGET_NAME_EXE}

        -Wl,--start-group
        pthread
        -Wl,--end-group
)

target_link_libraries(
        ${TARGET_NAME_EXE2}

        -Wl,--start-group
        pthread
        -Wl,--end-group
)

