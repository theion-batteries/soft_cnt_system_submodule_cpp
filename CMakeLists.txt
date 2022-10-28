############################
###### Cmake template ######
############################

# author: sami dhiab
# email: sami@theion.de

cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# project name and version: change project name
project(cnt_system VERSION "1.0")

# set the C++ standard: 20 is set
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# toolchain vcpkg
#This is necessary for MSVC to create a symbol file, .lib, besides a shared library, .dll
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

########################## add subdirectory##################

set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows/share")
#find_package(yaml-cpp CONFIG REQUIRED)
find_package(sockpp CONFIG REQUIRED)


#add_subdirectory(dependencies/sockpp)
if(NOT TARGET yaml-cpp)
        add_subdirectory(dependencies/yaml-cpp)
        install(TARGETS yaml-cpp)  
endif()

add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)  

###################### find library #############################


###################### Build Library ############################
# add all source *.cpp : change repo name
file(GLOB lib_SRCS
"${PROJECT_SOURCE_DIR}/src/cnt_controller.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_high_voltage.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_motion.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_dispenser.cpp"
)
# to create a static lib
add_library(cnt_system_static STATIC ${lib_SRCS}) 
target_link_libraries(cnt_system_static  
PUBLIC yaml-cpp
PUBLIC ${SOCKPP_LIBRARIES}
)

# includes
target_include_directories(cnt_system_static
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes/
PUBLIC ${SOCKPP_INCLUDE_DIRS}

)
############################# build executable  ###################################

# add all source *.cpp : change repo name
file(GLOB exe_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )
# make the exec: change project name
add_executable(cnt_system_exe ${exe_SRCS})

target_link_libraries(cnt_system_exe  
PUBLIC yaml-cpp
PUBLIC ${SOCKPP_LIBRARIES}
)

# includes
target_include_directories(cnt_system_exe
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes/
PUBLIC ${SOCKPP_INCLUDE_DIRS}

)

