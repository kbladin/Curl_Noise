# Some functions require a certain version of cmake (for example file GLOB )
cmake_minimum_required(VERSION 2.8)

# Create the project
set(PROJECT_NAME curlnoise)
project(${PROJECT_NAME})

# Add the external module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)

# Add the SGE subdirectory
add_subdirectory(${PROJECT_SOURCE_DIR}/ext/SimpleGraphicsEngine)

# Find external packages
find_package(OPENGL 		REQUIRED)
find_package(GLEW 			REQUIRED)
find_package(GLFW3 			REQUIRED)
find_package(ANTTWEAKBAR 	REQUIRED)
find_package(GLM 			REQUIRED)
if(APPLE)
  find_library(OPENGL_FRAMEWORK OpenGL)
  find_library(COCOA_FRAMEWORK Cocoa)
endif(APPLE)

# Add include directories
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${ANT_TWEAK_BAR_INCLUDE_PATH})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${SGE_INCLUDE_DIRS})

# Our own include directory
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

# Debugger flag
set(CMAKE_CXX_FLAGS "-g")

# Creating our own CURL_NOISE library
file(GLOB CURL_NOISE_lib_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB CURL_NOISE_lib_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
# We do not want main.cpp in the library
file(GLOB to_remove ${PROJECT_SOURCE_DIR}/src/main.cpp)
list(REMOVE_ITEM CURL_NOISE_lib_SOURCES ${to_remove})
list(REMOVE_ITEM CURL_NOISE_lib_HEADERS ${to_remove})
add_library(${PROJECT_NAME}_lib ${CURL_NOISE_lib_SOURCES} ${CURL_NOISE_lib_HEADERS})

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# C++11 compatability
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11")

# Link all the libraries (including our own)
target_link_libraries(${PROJECT_NAME}
	${PROJECT_NAME}_lib
	SGE
	${OPENGL_LIBRARIES}
	${OPENGL_glu_LIBRARY}
	${GLEW_LIBRARIES}
	${GLFW_LIBRARIES}
	${ANT_TWEAK_BAR_LIBRARY})

# OpenGL library (only for osx)
if(APPLE)
  target_link_libraries(${PROJECT_NAME} ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK})
endif(APPLE)
