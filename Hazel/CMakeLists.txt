cmake_minimum_required(VERSION 3.16)

project(Hazel LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)


#FILES
set (source_dir "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}")
file (GLOB source_files "${source_dir}/*.cpp"
                        "${source_dir}/Renderer/*.cpp")
file (GLOB header_files "${source_dir}/*.h"
                        "${source_dir}/Events/*.h"
                        "${source_dir}/Renderer/*.h")

target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files} ${events_header_files})

if(WIN32)
  target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/Platform/Windows/WindowsWindow.cpp")
  target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/Platform/Windows/WindowsInput.cpp")
endif()
if(UNIX AND NOT APPLE)
  target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/Platform/Linux/LinuxWindow.cpp")
  target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/Platform/Linux/LinuxInput.cpp")
endif()

target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL/OpenGLContext.cpp"
                                       "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL/OpenGLBuffer.cpp"
                                       "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL/OpenGLRendererAPI.cpp"
                                       "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL/OpenGLVertexArray.cpp"
                                       "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL/OpenGLShader.cpp")

target_sources(${PROJECT_NAME} PRIVATE "${source_dir}/ImGui/ImGuiLayer.cpp"
                                        "${source_dir}/ImGui/ImGuiLayer.h"
                                        "${source_dir}/ImGui/ImGuiBuild.cpp")


#PRECOMPILED HEADERS (cmake automatic includes to all source files, only available from 3.16)
target_precompile_headers(${PROJECT_NAME} PUBLIC [["hzpch.h"]])

#INCLUDE DIRS
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src/")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/vendor/spdlog/include/")

#DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_PLATFORM_$<UPPER_CASE:$<PLATFORM_ID>>")
target_compile_definitions(${PROJECT_NAME} PRIVATE "HZ_$<UPPER_CASE:$<CONFIG>>")
target_compile_definitions(${PROJECT_NAME} PRIVATE HZ_BUILD_DLL)
#target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# add_custom_target(print
#         ${CMAKE_COMMAND} -E echo "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/"

#GLFW DEPENDENCY
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
if (MSVC)
  set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/vendor/GLFW")
target_link_libraries(Hazel PRIVATE glfw)

#GLAD
add_subdirectory("${PROJECT_SOURCE_DIR}/vendor/glad")
target_link_libraries(Hazel PRIVATE glad)

#imgui
add_subdirectory("${PROJECT_SOURCE_DIR}/vendor/imgui")
target_link_libraries(Hazel PRIVATE imgui)

#glm
add_subdirectory("${PROJECT_SOURCE_DIR}/vendor/glm")
target_link_libraries(Hazel PUBLIC glm)
#making it public since we are using as a header library. It automatically includes in Sandbox 

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_definitions(${PROJECT_NAME} PRIVATE "_CRT_SECURE_NO_WARNINGS")
    # find_package(OpenGL REQUIRED)
    # target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
    # target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_gl_LIBRARY})
endif()
if(UNIX AND NOT APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:-O0>")
    # target_link_libraries(Hazel PUBLIC -lGL) #adding a hardcoded version for the time being
endif()

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/$<PLATFORM_ID>")

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}"
  CXX_STANDARD              17
)