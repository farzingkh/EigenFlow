cmake_minimum_required(VERSION 3.11.3)

# set(CMAKE_CXX_STANDARD 17)
project(NeuralNets)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")
 set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV 4.1 REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(${OpenCV_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/../eigen-3.3.7/" "/include/")
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

# Add project executable
add_executable(NN ${project_SRCS})
target_link_libraries(NN ${OpenCV_LIBRARIES})
target_link_libraries (NN Eigen3::Eigen)