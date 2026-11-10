cmake_minimum_required(VERSION 3.25)
project(vector VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(TEST_TARGET tests)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wcast-qual -std=c99")
set(SANITIZER_FLAGS -fsanitize=address,undefined)

set_source_files_properties(vector.c PROPERTIES COMPILE_OPTIONS -pedantic)

add_library(${PROJECT_NAME} STATIC vector.c)
target_compile_options(${PROJECT_NAME} PRIVATE -O2)

if (BUILD_TESTING)
    enable_testing()

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS test/*.c)
    add_executable(${TEST_TARGET} ${TEST_SOURCES} vector.c)
    target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_SOURCE_DIR})
    target_compile_options(${TEST_TARGET} PRIVATE ${SANITIZER_FLAGS})
    target_link_options(${TEST_TARGET} PRIVATE ${SANITIZER_FLAGS})

    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endif ()