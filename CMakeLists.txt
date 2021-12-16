cmake_minimum_required(VERSION 3.13)
project(safeSave)


add_library(safeSave)
target_include_directories(safeSave PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_sources(safeSave PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/safeSave.cpp")


target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/demo/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./demo/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the game


#Define MY_SOURCES to be a list of all the source files for my game 
file(GLOB_RECURSE PROFILER_LIB_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/demo/*.cpp")
add_executable(safeSave_test)
target_sources(safeSave_test PRIVATE ${PROFILER_LIB_SOURCES} )
target_link_libraries(safeSave_test PUBLIC safeSave)

