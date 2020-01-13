cmake_minimum_required(VERSION 3.16)

project(Hazel LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED)


#FILES
set (source_dir "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}")
file (GLOB source_files "${source_dir}/*.cpp")
file (GLOB header_files "${source_dir}/*.h"
                        "${source_dir}/Events/*.h")

target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files} ${events_header_files})

if(WIN32)
  target_sources(${PROJECT_NAME} PRIVATE "${source_dir}/Platform/Windows/WindowsWindow.cpp")
endif()
if(UNIX AND NOT APPLE)
  target_sources(${PROJECT_NAME} PRIVATE "${source_dir}/Platform/Linux/LinuxWindow.cpp")
endif()

#PRECOMPILED HEADERS (cmake automatic includes to all source files, only available from 3.16)
target_precompile_headers(${PROJECT_NAME} PUBLIC [["hzpch.h"]])

#INCLUDE DIRS
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src/")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/vendor/spdlog/include/")

#DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_PLATFORM_$<UPPER_CASE:$<PLATFORM_ID>>")
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_$<UPPER_CASE:$<CONFIG>>")
target_compile_definitions(${PROJECT_NAME} PRIVATE HZ_BUILD_DLL)

# add_custom_target(print
#         ${CMAKE_COMMAND} -E echo "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/"

#GLFW DEPENDENCY
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${PROJECT_SOURCE_DIR}/vendor/GLFW")

target_link_libraries(Hazel PRIVATE glfw)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    find_package(OpenGL REQUIRED)
    target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_gl_LIBRARY})
endif()
if(UNIX AND NOT APPLE)
    target_link_libraries(Hazel PUBLIC -lGL) #adding a hardcoded version for the time being
endif()

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/$<PLATFORM_ID>")

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  CXX_STANDARD              17
)