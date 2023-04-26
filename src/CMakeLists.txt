if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_ImageProcess_Core/CMakeLists.txt")
    add_subdirectory(BionicEyes_ImageProcess_Core)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_ImageProcess_Core/CMakeLists.txt")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_Base/CMakeLists.txt")
    add_subdirectory(BionicEyes_Base)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_Base/CMakeLists.txt")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EagleEyes_Base/CMakeLists.txt")
    add_subdirectory(EagleEyes_Base)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EagleEyes_Base/CMakeLists.txt")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/HumanEyes_Base/CMakeLists.txt")
    add_subdirectory(HumanEyes_Base)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/HumanEyes_Base/CMakeLists.txt")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_SDK/CMakeLists.txt")
    add_subdirectory(BionicEyes_SDK)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_SDK/CMakeLists.txt")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_APP/CMakeLists.txt")
    add_subdirectory(BionicEyes_APP)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_APP/CMakeLists.txt")

if(BUILD_SAMPLES MATCHES ON)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_Samples/CMakeLists.txt")
    add_subdirectory(BionicEyes_Samples)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_Samples/CMakeLists.txt")
endif()

if(BUILD_TEST MATCHES ON)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_Test/CMakeLists.txt")
    add_subdirectory(BionicEyes_Test)
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/BionicEyes_Test/CMakeLists.txt")
endif()
