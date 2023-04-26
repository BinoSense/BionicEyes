#get all subdirs of curdir
macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

#group srcs (use the parent folder as the group name for src files)
macro(GROUP_SRCS SRCS_FILES)
	foreach(SRC IN LISTS SRCS_FILES)
		get_filename_component(SRC_PATH "${SRC}" PATH)
		file(RELATIVE_PATH REL_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${SRC_PATH}")
		if(NOT ${REL_SRC_PATH} STREQUAL "")
			string(REPLACE "/" "\\" REL_SRC_PATH ${REL_SRC_PATH})
			source_group("${REL_SRC_PATH}" FILES "${SRC}")
		endif()
	endforeach()
endmacro()

macro(CHECK_PRECOMPILED_HEADER name)
	file(GLOB_RECURSE PCH_FILES "_*_precompiled.h")
	if(PCH_FILES AND USE_PCH)
		message("${name} enable PCH, file:${PCH_FILES}")
		set_target_properties(${name} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
		set_target_properties(${name} PROPERTIES COTIRE_ENABLE_PRECOMPILED_HEADER TRUE)
		set_target_properties(${name} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${PCH_FILES})
		cotire(${name})
	endif(PCH_FILES AND USE_PCH)
endmacro()

#macro(CONFIG_LIB_MODULE name type)
#	string(TOUPPER ${name} expname)
#	string(TOUPPER ${type} exptype)
#	set(ExtraMacroArgs ${ARGN})
#	# Get the length of the list
#	list(LENGTH ExtraMacroArgs NumExtraMacroArgs)
#	if(NumExtraMacroArgs GREATER 0)
#		list (GET ExtraMacroArgs 0 folder)
#		set(DESTDIR ${folder})
#		if(${folder} MATCHES "export")
#			#export libs to users
#			set_target_properties(${name}
#				PROPERTIES
#				DEFINE_SYMBOL ${expname}_EXPORTS
#				ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_PATH}
#				PUBLIC_HEADER ${name}.h)
#			add_custom_command(TARGET ${name}
#				POST_BUILD
#				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h ${INCLUDE_PATH}
#			)
#			install(TARGETS ${name}
#				EXPORT "${CMAKE_PROJECT_NAME}" #need add "PRIVATE" before dependence libs in target_link_libraries command
#				ARCHIVE DESTINATION lib
#				RUNTIME DESTINATION bin
#				LIBRARY DESTINATION bin
#				PUBLIC_HEADER DESTINATION include)
#		elseif(${folder} MATCHES "internal")
#			#internal libs, only export dlls to users
#			set_target_properties(${name}
#				PROPERTIES
#				DEFINE_SYMBOL ${expname}_EXPORTS
#				ARCHIVE_OUTPUT_DIRECTORY ${INTERNAL_LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${INTERNAL_LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${INTERNAL_LIB_PATH})
#			add_custom_command(TARGET ${name}
#				POST_BUILD
#				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h ${INTERNAL_INCLUDE_PATH}
#			)
#			#for internal libs, only install shared library to bin
#			if(${type} STREQUAL "SHARED")
#				install(TARGETS ${name}	RUNTIME DESTINATION bin LIBRARY DESTINATION bin)
#			endif(${type} STREQUAL "SHARED")
#		elseif(${folder} MATCHES "enhance")
#			#enhance modules, only install these modules when demo app is installed
#			set_target_properties(${name}
#				PROPERTIES
#				DEFINE_SYMBOL ${expname}_EXPORTS
#				ARCHIVE_OUTPUT_DIRECTORY ${INTERNAL_LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${INTERNAL_LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${INTERNAL_LIB_PATH})
#			add_custom_command(TARGET ${name}
#				POST_BUILD
#				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h ${INTERNAL_INCLUDE_PATH}
#			)
#			if(INSTALL_DEMOS)
#				if(${type} STREQUAL "SHARED")
#					install(TARGETS ${name}	RUNTIME DESTINATION bin LIBRARY DESTINATION bin)
#				endif(${type} STREQUAL "SHARED")
#			endif(INSTALL_DEMOS)
#		else(${folder} MATCHES "export")
#			#others, not export to users, only for test or under develop modules
#			set_target_properties(${name}
#				PROPERTIES
#				DEFINE_SYMBOL ${expname}_EXPORTS
#				ARCHIVE_OUTPUT_DIRECTORY ${INTERNAL_LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${INTERNAL_LIB_PATH}
#				ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${INTERNAL_LIB_PATH})
#			add_custom_command(TARGET ${name}
#				POST_BUILD
#				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h ${INTERNAL_INCLUDE_PATH}
#			)
#		endif(${folder} MATCHES "export")
#
#	#when no extra args set default as export lib
#	else()
#		message(FATAL_ERROR "Parent Folder Name should be set when config a library module")
#	endif()
#	if(UNIX AND ${type} STREQUAL "STATIC")
#		set_target_properties (${name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
#		set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wl,--exclude-libs,ALL ")
#	elseif(UNIX AND ${type} STREQUAL "SHARED")
#		set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wl,--exclude-libs,ALL ")
#	endif(UNIX AND ${type} STREQUAL "STATIC")
#endmacro()

macro(CONFIG_LIB_MODULE name type)
	string(TOUPPER ${name} expname)
	string(TOUPPER ${type} exptype)
	set(ExtraMacroArgs ${ARGN})
	# Get the length of the list
	list(LENGTH ExtraMacroArgs NumExtraMacroArgs)
	if(NumExtraMacroArgs GREATER 0)
		list (GET ExtraMacroArgs 0 folder)
		set(DESTDIR ${folder})

		#export libs to users
		set_target_properties(${name}
				PROPERTIES
				DEFINE_SYMBOL ${expname}_EXPORTS
				ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH}
				ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIB_PATH}
				ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_PATH}
				#PUBLIC_HEADER ${name}.h
				)
#		add_custom_command(TARGET ${name}
#				POST_BUILD
#				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h ${INCLUDE_PATH}
#				)
		install(TARGETS ${name}
				EXPORT "${CMAKE_PROJECT_NAME}" #need add "PRIVATE" before dependence libs in target_link_libraries command
				ARCHIVE DESTINATION lib
				RUNTIME DESTINATION bin
				LIBRARY DESTINATION bin
				#PUBLIC_HEADER DESTINATION include
				)

		#when no extra args set default as export lib
	else()
		message(FATAL_ERROR "Parent Folder Name should be set when config a library module")
	endif()
	if(UNIX AND ${type} STREQUAL "STATIC")
		set_target_properties (${name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
		set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wl,--exclude-libs,ALL ")
	elseif(UNIX AND ${type} STREQUAL "SHARED")
		set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wl,--exclude-libs,ALL ")
	endif(UNIX AND ${type} STREQUAL "STATIC")
endmacro()

#add optional parameter make it a InternalLib
macro(ADD_LIB_MODULE name type)
	file(GLOB_RECURSE SRCS_FILES "*.h" "*.cpp" "*.c" "*.cl" "*.S")
	if(NOT type)
		set(type SHARED)
	endif()
	add_library(${name} ${type} ${SRCS_FILES})
	
	GROUP_SRCS(${SRCS_FILES})
	CONFIG_LIB_MODULE(${name} ${type} ${ARGN})
	CHECK_PRECOMPILED_HEADER(${name})
endmacro()
#optional parameter isInternalLib
macro(ADD_CUDALIB_MODULE name type)
	file(GLOB_RECURSE SRCS_FILES "*.h" "*.cpp" "*.c" "*.cu" "*.S")
	if(NOT type)
		set(type SHARED)
	endif()
	string(TOUPPER ${name} expname)
	# nvcc flags;
	set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-O3 ${GENCODE} -D${expname}_EXPORTS)
	if(WIN32)
		set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-Xcompiler \"/wd 4819\"")
	endif(WIN32)
	cuda_add_library(${name} ${type} ${SRCS_FILES})
	
	GROUP_SRCS(${SRCS_FILES})
	CONFIG_LIB_MODULE(${name} ${type} ${ARGN})
endmacro()


macro(CONFIG_EXE_MODULE name)
	string(TOUPPER ${name} expname)
	set(ExtraMacroArgs ${ARGN})
	# Get the length of the list
	list(LENGTH ExtraMacroArgs NumExtraMacroArgs)
	if(NumExtraMacroArgs GREATER 0)	
		list (GET ExtraMacroArgs 0 folder)
		set(DESTDIR ${folder})
		if(WIN32)
			#need copy qt dlls platform folder and other dll dependencies to DESTDIR on windows
			set(DESTDIR bin)
		endif(WIN32)

#		if(${folder} STREQUAL "samples")
			file(GLOB_RECURSE ALL_FILES "*.*")
			install(FILES ${ALL_FILES} DESTINATION ${folder}/${name}/src)
			install(TARGETS ${name}	RUNTIME DESTINATION ${folder}/${name}/bin)
#		elseif(${folder} STREQUAL "apps")
#			#install program to apps folder
#			install(TARGETS ${name}	RUNTIME DESTINATION ${DESTDIR})
#			if(USE_CPACK)
#				#add shortcuts to start menu, cmake cannnot use global vars, use cache instead, need clear at the end.
#				set(CPACK_PACKAGE_EXECUTABLES ${CPACK_PACKAGE_EXECUTABLES} ${name} "${name}"	CACHE INTERNAL "CPACK_PACKAGE_EXECUTABLES")
#			endif(USE_CPACK)
#		elseif(${folder} STREQUAL "demos")
#			if(INSTALL_DEMOS)
#				install(TARGETS ${name}	RUNTIME DESTINATION ${DESTDIR})
#			endif(INSTALL_DEMOS)
#		else(${folder} STREQUAL "samples")#tests
#			if(INSTALL_TESTS)
#				install(TARGETS ${name}	RUNTIME DESTINATION ${DESTDIR})
#			endif(INSTALL_TESTS)
#		endif(${folder} STREQUAL "samples")
#	else()
		#not install
	endif()
endmacro()

macro(ADD_EXE_MODULE name)
	file(GLOB_RECURSE SRCS_FILES "*.h" "*.cpp" "*.c" "*.ui" "*.rc")
	file(GLOB_RECURSE RESOURCE_FILES "*.qrc")
	if(RESOURCE_FILES)
		QT5_ADD_RESOURCES(RESOURCES ${RESOURCE_FILES})
	endif()
	add_executable(${name} ${SRCS_FILES} ${RESOURCES})
	
	GROUP_SRCS(${SRCS_FILES})
	CONFIG_EXE_MODULE(${name} ${ARGN})
	CHECK_PRECOMPILED_HEADER(${name})
endmacro()

macro(ADD_CUDAEXE_MODULE name)
	file(GLOB_RECURSE SRCS_FILES "*.h" "*.cuh" "*.cpp" "*.c" "*.cu" "*.ui" "*.rc")
	file(GLOB_RECURSE RESOURCE_FILES "*.qrc")
	if(RESOURCE_FILES)
		QT5_ADD_RESOURCES(RESOURCES ${RESOURCE_FILES})
	endif()
	# nvcc flags;
	set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-O3  ${GENCODE})
	
	if(WIN32)
		set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-Xcompiler \"/wd 4819\"")
	endif(WIN32)	
	
	cuda_add_executable(${name} ${SRCS_FILES} ${RESOURCES})
	GROUP_SRCS(${SRCS_FILES})
	CONFIG_EXE_MODULE(${name} ${ARGN})
endmacro()
