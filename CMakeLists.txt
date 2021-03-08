cmake_minimum_required(VERSION 3.10)

# set the project name
project(CGHomework1)

# add packages
find_package(OpenGL REQUIRED)
find_library(GLEW NAMES glew32s PATHS vendor/lib/Win32)
find_library(GLFW NAMES glfw3 PATHS vendor/lib)

# include and lib dirs
set(INCLUDE_DIRS vendor/include)
set(LIBRARY_DIRS vendor/lib)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS 
   "/wd4018 /wd4244 /wd4305 
   /D_CRT_SECURE_NO_WARNINGS 
   /D_CRT_NONSTDC_NO_DEPRECATE 
   /D NOMINMAX
   /EHsc
   /ENABLE_ASSERTS")

set(CORE ${GLEW} ${GLFW} opengl32.lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${INCLUDE_DIRS})
link_directories(${LIBRARY_DIRS})
add_subdirectory(sandbox)