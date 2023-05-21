cmake_minimum_required(VERSION 3.26.3 FATAL_ERROR)

set(PROJECT_NAME BattleCity)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/Renderer/ShaderProgramm.cpp
	src/Renderer/ShaderProgramm.h
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external_lib/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external_lib/glad)
target_link_libraries(${PROJECT_NAME} glad)