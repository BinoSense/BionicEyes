set(archdetect_c_code "
#if defined(__i386__) || defined(_M_IX86)
	#error BZ_BIT 32
#endif
#if defined(__x86_64__) || defined(_M_X64)
	#error BZ_BIT 64
#endif
#if defined(__arm__)
	#error BZ_BIT arm32
#endif
#if defined(__arm64__) || defined(__aarch64__)
	#error BZ_BIT arm64
#endif
#error BZ_BIT unknown
")

function(get_architecture output_var)
	file(WRITE "${CMAKE_BINARY_DIR}/arch.c" "${archdetect_c_code}")
	enable_language(C)
	try_run(
		run_result_unused
		compile_result_unused
		"${CMAKE_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}/arch.c"
		COMPILE_OUTPUT_VARIABLE ARCH
		CMAKE_FLAGS CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
	)
	string(REGEX MATCH "BZ_BIT ([a-zA-Z0-9_]+)" ARCH "${ARCH}")
	string(REPLACE "BZ_BIT " "" ARCH "${ARCH}")
	string(REPLACE "arm" "" ARCH "${ARCH}")
    set(${output_var} "${ARCH}" PARENT_SCOPE)
endfunction()

function(is_archarm output_var)
	file(WRITE "${CMAKE_BINARY_DIR}/arch.c" "${archdetect_c_code}")
	enable_language(C)
	try_run(
		run_result_unused
		compile_result_unused
		"${CMAKE_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}/arch.c"
		COMPILE_OUTPUT_VARIABLE ARCH
		CMAKE_FLAGS CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
	)
	string(REGEX MATCH "BZ_BIT ([a-zA-Z0-9_]+)" ARCH "${ARCH}")
	if("${ARCH}" MATCHES "arm")
		set(OUTVAR TRUE)
	else()
		set(OUTVAR FALSE)
	endif()
    set(${output_var} "${OUTVAR}" PARENT_SCOPE)
endfunction()
