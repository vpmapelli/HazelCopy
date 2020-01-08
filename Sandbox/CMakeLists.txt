cmake_minimum_required(VERSION 3.5)

project(Sandbox LANGUAGES CXX)

add_executable(${PROJECT_NAME})

#FILES
set (source_dir "${PROJECT_SOURCE_DIR}/src/")
file (GLOB source_files "${source_dir}/*.cpp")
file (GLOB header_files "${source_dir}/*.h")

target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files})

#INCLUDES
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/../Hazel/src/")

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
