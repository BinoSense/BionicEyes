 set(TARGET_NAME evo_be_Sample_GrabImage_Local)
 set(TARGET_NAME_EXE "${TARGET_NAME}_${UBUNTU_DISTRIB_RELEASE}")
 find_package(OpenCV_NoCUDA REQUIRED)

 include_directories(${OpenCV_INCLUDE_DIRS})

 include_directories(${PROJECT_PATH}/include ${PROJECT_PATH}/include/internal)

 link_directories("/opt/pylon5/lib64")

 add_executable(${TARGET_NAME_EXE}  BE_Sample_GrabImage_Local.cpp)

 target_link_libraries(
         ${TARGET_NAME_EXE}

         -Wl,--start-group
         ${OpenCV_LIBS}
         evo_be_Internal_shared_${UBUNTU_DISTRIB_RELEASE}
         evo_be_ImageProcess_Core_shared_${UBUNTU_DISTRIB_RELEASE}
         evo_be_Device_5_shared_${UBUNTU_DISTRIB_RELEASE}
         pthread
         evo_be_Communication_shared_${UBUNTU_DISTRIB_RELEASE}
         X11
         -Wl,--end-group
 )
