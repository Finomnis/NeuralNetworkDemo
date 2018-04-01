cmake_minimum_required(VERSION 3.5)

project(NeuralNetworkDemo)
set (CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic-errors")

# Search for dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

# Set include paths of dependencies
include_directories(${SDL2_INCLUDE_DIR})

# Create executable
file(GLOB_RECURSE SRC "src/*.cxx" "src/*.hxx")
add_executable(NeuralNetworkDemo ${SRC})

# Link executable to dependencies
target_link_libraries(NeuralNetworkDemo ${SDL2_LIBRARY})
