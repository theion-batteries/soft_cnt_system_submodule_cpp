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

#This is necessary for MSVC to create a symbol file, .lib, besides a shared library, .dll
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# add dependencies cmake
#add_subdirectory(dependencies/sockpp)
#add_subdirectory(dependencies/yaml-cpp)

# includes
include_directories(${PROJECT_SOURCE_DIR}/includes/)
# dependencies manual includes: change repo name
include_directories(${PROJECT_SOURCE_DIR}/dependencies/sockpp/include/)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/yaml-cpp/include/)

# add all source *.cpp : change repo name
file(GLOB exe_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )
# add all source *.cpp : change repo name
file(GLOB lib_SRCS
"${PROJECT_SOURCE_DIR}/src/cnt_controller.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_high_voltage.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_motion.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_dispenser.cpp"
)
# to create a static lib
add_library(cnt_system_static STATIC ${lib_SRCS}) 

# make the exec: change project name
add_executable(cnt_system_exe ${exe_SRCS})

# link againt static libs
target_link_libraries(cnt_system_exe ${PROJECT_SOURCE_DIR}/build/dependencies/yaml-cpp/Debug/yaml-cppd.lib
                                 ${PROJECT_SOURCE_DIR}/build/dependencies/sockpp/Debug/sockpp-static.lib
)
target_link_libraries(cnt_system_static  ${PROJECT_SOURCE_DIR}/build/dependencies/yaml-cpp/Debug/yaml-cppd.lib
                                         ${PROJECT_SOURCE_DIR}/build/dependencies/sockpp/Debug/sockpp-static.lib
)

