cmake_minimum_required(VERSION 3.5)

project(Hazel LANGUAGES CXX)


#include_directories()

set (source_dir "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}")
file (GLOB source_files "${source_dir}/*.cpp")
file (GLOB header_files "${source_dir}/*.h")

message(STATUS ${source_files})

add_library(${PROJECT_NAME} SHARED)
target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files})


#DEFINE HZ_PLATFORM_{LINUX|WINDOWS}
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_PLATFORM_$<UPPER_CASE:$<PLATFORM_ID>>")
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_$<UPPER_CASE:$<CONFIG>>")
target_compile_definitions(${PROJECT_NAME} PRIVATE HZ_BUILD_DLL)

add_custom_target(print
        ${CMAKE_COMMAND} -E echo "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/"
    )

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/$<PLATFORM_ID>")

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  CXX_STANDARD              17
)