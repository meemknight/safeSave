cmake_minimum_required(VERSION 3.16)
project(safeSave)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(safeSave)
target_include_directories(safeSave PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_sources(safeSave PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/safeSave.cpp")

add_executable(safeSave_test)


target_compile_definitions(safeSave_test PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/demo/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions(safeSave_test PUBLIC RESOURCES_PATH="./demo/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the game


target_sources(safeSave_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/demo/test.cpp" )
target_link_libraries(safeSave_test PUBLIC safeSave)

