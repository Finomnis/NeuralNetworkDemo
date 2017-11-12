cmake_minimum_required(VERSION 3.5)

project(NeuralNetworkDemo)
set (CMAKE_CXX_STANDARD 11)

# Search for dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

# Set include paths of dependencies
include_directories(${SDL2_INCLUDE_DIR})

# Create executable
set(SRC
  src/main.cxx
  src/SDL2Windows.hxx
  src/SDL2Windows.cxx
  src/Log.hxx
  src/Log.cxx
)
add_executable(NeuralNetworkDemo ${SRC})

# Link executable to dependencies
target_link_libraries(NeuralNetworkDemo ${SDL2_LIBRARY})
