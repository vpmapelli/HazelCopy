cmake_minimum_required(VERSION 3.16)

project(glad LANGUAGES C)

add_library(${PROJECT_NAME} STATIC)

#FILES
target_sources(${PROJECT_NAME} PRIVATE 
                                "${PROJECT_SOURCE_DIR}/src/glad.c" 
                                "${PROJECT_SOURCE_DIR}/include/glad/glad.h" 
                                "${PROJECT_SOURCE_DIR}/include/KHR/khrplatform.h")

#INCLUDE DIRS
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

#LINKING
if(UNIX AND NOT APPLE)
target_link_libraries(${PROJECT_NAME} PUBLIC -ldl)
endif()
if (WIN32)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# message(STATUS "GLAD $<$<CONFIG:Debug>:Debug>")

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
