cmake_minimum_required(VERSION 3.28)
project(LUDA C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS src/*.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-command-line-argument")

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/vendor)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -pedantic-errors -Wall -Werror -Wextra -Wbad-function-cast -Wcast-function-type)

# sanitizers
target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all )
target_link_libraries(${PROJECT_NAME} "-fsanitize=address,undefined")

include(cmake/lua.cmake)

if (EMSCRIPTEN)
    set(PLATFORM Web)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    add_definitions(-DPLATFORM_WEB)
    add_definitions(-DUSE_WASM)

    file(COPY examples DESTINATION ${CMAKE_BINARY_DIR}/)
    
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy -s GL_ENABLE_GET_PROC_ADDRESS -s USE_GLFW=3 -s FETCH=1 -lidbfs.js --embed-file examples/ --shell-file ${CMAKE_SOURCE_DIR}/src/index.html")

    string(REPLACE "-rdynamic" "" CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_subdirectory(${THIRDPARTY_DIR}/raylib)

target_include_directories(${PROJECT_NAME} PUBLIC SYSTEM vendor/raylib/src)
target_link_libraries(${PROJECT_NAME} raylib lua)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
