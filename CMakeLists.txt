cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
project(simpleqm)
if(USE_CONAN)
	set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
endif()
file(GLOB SOURCES
	src/quine-mccluskey.hpp
	src/quine-mccluskey.cpp
)
list(APPEND SOURCES "src/main.cpp")
add_executable(simpleqm ${SOURCES})