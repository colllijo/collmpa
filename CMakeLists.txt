cmake_minimum_required(VERSION 3.28)

project(CollMPA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
set(CLANG_FORMAT_BIN_NAME clang-format)
set(CLANG_FORMAT_EXCLUDE_PATTERNS "build/" ${CMAKE_BINARY_DIR})

find_package(CLANG_FORMAT)

set(SOURCE_DIR "src")
set(INCLUDE_DIR "include")

set(SOURCES
  "${SOURCE_DIR}/main.cpp"
)

set(HEADERS
)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
