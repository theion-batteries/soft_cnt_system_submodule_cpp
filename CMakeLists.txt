############################
###### Cmake template ######
############################

# author: sami dhiab
# email: sami@theion.de

cmake_minimum_required(VERSION 3.1.0)

# project name and version: change project name
project(cnt_system VERSION "1.0")
 
# set the c++ standard: 20 is set
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()
# toolchain vcpkg
#This is necessary for MSVC to create a symbol file, .lib, besides a shared library, .dll
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
#add_subdirectory(tests)

########################## add subdirectory##################
set(CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/x64-windows/share")
find_package(sockpp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)  
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)  
###################### find library #############################
find_file(cnt_config 
        NAMES cnt_config.yaml
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}/config
        REQUIRED
)
message("found cnt config file: ${cnt_config}")        
set ( CNT_CONFIG "${cnt_config}" CACHE FILEPATH "${cnt_config}")
set(CNT_CONFIG_DIR ${CMAKE_INSTALL_PREFIX}/config/cnt_config.yaml CACHE FILEPATH "${CMAKE_INSTALL_PREFIX}/config/cnt_config.yaml}") 
add_definitions(-DCNT_CONFIG="${CNT_CONFIG_DIR}")

# copy config one by one 
function(copy_cnt_config target_exe config_file)
add_custom_command(TARGET ${target_exe} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy ${config_file} ${CNT_CONFIG_DIR}  
COMMENT "copy config from ${config_file} to ${CNT_CONFIG_DIR}"
    )
endfunction()
# copy dlls one by one 
function(copy_dlls_yaml_target_destination target_exe)
add_custom_command(TARGET ${target_exe} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different  $<TARGET_FILE:yaml-cpp> $<TARGET_FILE_DIR:${target_exe}>
COMMAND_EXPAND_LISTS)
endfunction()
###################### Build Library ############################
# add all source *.cpp : change repo name
file(GLOB lib_SRCS
"${PROJECT_SOURCE_DIR}/src/cnt_controller.cpp"
"${PROJECT_SOURCE_DIR}/src/Icnt_high_voltage.cpp"
"${PROJECT_SOURCE_DIR}/src/Icnt_axis_motion.cpp"
"${PROJECT_SOURCE_DIR}/src/Icnt_dispensing.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_high_voltage_gbs.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_linear_motion.cpp"
"${PROJECT_SOURCE_DIR}/src/cnt_dispenser_vibration.cpp"

)
# to create a static lib
add_subdirectory(tests)
add_library(cntLib STATIC ${lib_SRCS}) 
target_link_libraries(cntLib  
PUBLIC wsock32 
PUBLIC ws2_32
PUBLIC yaml-cpp
PUBLIC ${SOCKPP_LIBRARIES}
)
# includes
target_include_directories(cntLib
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/
PUBLIC ${SOCKPP_INCLUDE_DIRS}
)
copy_cnt_config(cntLib ${CNT_CONFIG})
############################# build executable  ###################################
# add all source *.cpp : change repo name
file(GLOB exe_SRCS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )
# make the exec: change project name
add_executable(cntBin ${exe_SRCS})
target_link_libraries(cntBin  
PUBLIC yaml-cpp
PUBLIC wsock32 
PUBLIC ws2_32
PUBLIC ${SOCKPP_LIBRARIES}
)
# includes
target_include_directories(cntBin
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes/
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/
PUBLIC ${SOCKPP_INCLUDE_DIRS}
)
copy_cnt_config(cntBin ${CNT_CONFIG})
copy_dlls_yaml_target_destination(cntBin)

############################ install targets ########################################
    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h")
    INSTALL(FILES ${files} DESTINATION include/cntLib/)
    INSTALL(FILES ${CNT_CONFIG} DESTINATION config/)
    install(TARGETS cntLib #cntBin
    CONFIGURATIONS Debug
    RUNTIME DESTINATION bin/Debug/
    LIBRARY DESTINATION lib)
