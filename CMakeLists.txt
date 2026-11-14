cmake_minimum_required(VERSION 3.25)
project(vector VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(TEST_TARGET tests)

set(WARNINGS -Wall -Wextra -Wcast-qual -Wsign-conversion)
set(SANITIZER_FLAGS -fsanitize=address,undefined)

set_source_files_properties(vector.c PROPERTIES COMPILE_OPTIONS -pedantic)

add_library(${PROJECT_NAME} STATIC vector.c)
target_compile_options(${PROJECT_NAME} PRIVATE
    ${WARNINGS}
    -Werror
    $<$<NOT:$<CONFIG:Debug>>:-O2>
)

if(BUILD_TESTING)
    enable_testing()

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS test/*.c)
    add_executable(${TEST_TARGET} EXCLUDE_FROM_ALL ${TEST_SOURCES} vector.c)
    target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_SOURCE_DIR})
    target_compile_options(${TEST_TARGET} PRIVATE
        ${WARNINGS}
        ${SANITIZER_FLAGS}
        -g # This is for sanitizer stack traces regardless of config
    )
    target_link_options(${TEST_TARGET} PRIVATE ${SANITIZER_FLAGS})

    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endif()