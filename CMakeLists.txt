############################
###### Cmake template ######
############################

# author: sami dhiab
# email: sami@theion.de

cmake_minimum_required(VERSION 3.5.0)

# project name and version: change project name
project(cnt_system VERSION "1.0")

# set the C++ standard: 20 is set
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add dependencies cmake
add_subdirectory(dependencies/sockpp)
add_subdirectory(dependencies/yaml-cpp)

# includes
include_directories(${PROJECT_SOURCE_DIR}/includes/)
# dependencies manual includes: change repo name
include_directories(${PROJECT_SOURCE_DIR}/dependencies/sockpp/include/)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/include/)

# add all source *.cpp : change repo name
file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )

# make the exec: change project name
add_executable(cnt_system ${all_SRCS})

# link againt static libs
target_link_libraries(cnt_system ${PROJECT_SOURCE_DIR}/build/dependencies/yaml-cpp/Debug/yaml-cppd.lib
)
target_link_libraries(cnt_system ${PROJECT_SOURCE_DIR}/build/dependencies/sockpp/Debug/sockpp-static.lib
)
