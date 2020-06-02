
cmake_minimum_required(VERSION 3.16)

project(stb_image LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

#FILES
target_sources(${PROJECT_NAME} PRIVATE 
                                "${PROJECT_SOURCE_DIR}/stb_image.h" 
                                "${PROJECT_SOURCE_DIR}/stb_image.cpp")

#INCLUDE DIRS
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}")

#LINKING
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# message(STATUS "GLAD $<$<CONFIG:Debug>:Debug>")

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
