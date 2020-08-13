cmake_minimum_required(VERSION 3.5)

project(HazelSolution LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    #set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
endif()

add_subdirectory(Hazel)

add_subdirectory(Sandbox)

add_dependencies(Sandbox Hazel)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
endif()

# target_link_libraries(Sandbox Hazel)