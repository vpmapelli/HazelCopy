cmake_minimum_required(VERSION 3.5)

project(HazelSolution LANGUAGES CXX)

add_subdirectory(Hazel)

add_subdirectory(Sandbox)

# target_link_libraries(Sandbox Hazel)