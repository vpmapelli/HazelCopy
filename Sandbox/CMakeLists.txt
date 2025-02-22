cmake_minimum_required(VERSION 3.16)

project(Sandbox LANGUAGES CXX)

add_executable(${PROJECT_NAME})

#FILES
set (source_dir "${PROJECT_SOURCE_DIR}/src/")
file (GLOB source_files "${source_dir}/*.cpp")
file (GLOB header_files "${source_dir}/*.h")

target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files})

#INCLUDE DIRS
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/../Hazel/src/")
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/../Hazel/vendor/")

#LINKING
target_link_libraries(${PROJECT_NAME} Hazel)

#DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_PLATFORM_$<UPPER_CASE:$<PLATFORM_ID>>") #Define Platform {WINDOWS|lINUX}
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_$<UPPER_CASE:$<CONFIG>>")               #Define CFG Type: {DEBUG|RELEASE}

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/$<PLATFORM_ID>/${PROJECT_NAME}"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/$<PLATFORM_ID>/${PROJECT_NAME}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/$<PLATFORM_ID>/${PROJECT_NAME}"
  CXX_STANDARD              17
)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)

endif()

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

	# add_custom_command(
	# 		TARGET ${PROJECT_NAME} PRE_BUILD              							 # Adds a pre-build event to Sandbox
	# 		COMMAND ${CMAKE_COMMAND} -E copy_if_different  							 # which executes "cmake - E copy_if_different..."
	# 		"${PROJECT_SOURCE_DIR}/../bin/$<CONFIG>/$<PLATFORM_ID>/Hazel/Hazel.dll"  # <--this is in-file
	# 		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/")                 				 	 # <--this is out-file path
elseif(UNIX)
	target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:-O0>")
	# add_custom_command(
	# 		TARGET ${PROJECT_NAME} PRE_BUILD              							# Adds a pre-build event to Sandbox
	# 		COMMAND ${CMAKE_COMMAND} -E copy_if_different  							# which executes "cmake - E copy_if_different..."
	# 		"${PROJECT_SOURCE_DIR}/../bin/$<CONFIG>/$<PLATFORM_ID>/Hazel/libHazel.so"  # <--this is in-file
	# 		$<TARGET_FILE_DIR:${PROJECT_NAME}>)                 					# <--this is out-file path
endif()