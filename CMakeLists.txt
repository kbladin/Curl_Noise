# Some functions require a certain version of cmake
cmake_minimum_required(VERSION 2.8)

# Create the project
set(PROJECT_NAME curlnoise)
project(${PROJECT_NAME})

# Add the external module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)

# Find external packages
find_package(OPENGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
#find_package(ANTTWEAKBAR REQUIRED)

set(ASSIMP_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ext/assimp/include)
set(ASSIMP_LIBRARIES ${PROJECT_SOURCE_DIR}/ext/assimp/lib/libassimp.dylib)

if(APPLE)
  # Find external libraries
  find_library(OPENGL_FRAMEWORK OpenGL)
  find_library(COCOA_FRAMEWORK Cocoa)
endif(APPLE)

# Add include directories 
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})
#include_directories(${ANT_TWEAK_BAR_INCLUDE_PATH})
include_directories(${ASSIMP_INCLUDE_DIRS})

# Our own include directory
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

set(CMAKE_CXX_FLAGS "-g")

# Creating our own library
file(GLOB Curl_Noise_lib_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/SGE/*.cpp)
file(GLOB Curl_Noise_lib_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/SGE/*.h)
# We do not want main.cpp in the library
file(GLOB to_remove ${PROJECT_SOURCE_DIR}/src/main.cpp)
list(REMOVE_ITEM Curl_Noise_lib_SOURCES ${to_remove})
list(REMOVE_ITEM Curl_Noise_lib_HEADERS ${to_remove})
add_library(Curl_Noise_lib ${Curl_Noise_lib_SOURCES} ${Curl_Noise_lib_HEADERS})

# Creating our executable
add_executable(curlnoise src/main.cpp)
# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere (In our case for testing).

# C++11 compatability
set_target_properties(curlnoise PROPERTIES COMPILE_FLAGS "-std=c++11")

# Link all the libraries (including our own)
target_link_libraries(curlnoise Curl_Noise_lib ${OPENGL_LIBRARIES} ${OPENGL_glu_LIBRARY} ${ASSIMP_LIBRARIES} ${GLEW_LIBRARY} ${GLFW_LIBRARIES})

######### OPENGL LIBRARY (ONLY FOR OSX) #############
if(APPLE)
  target_link_libraries(curlnoise ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK})
endif(APPLE)
