cmake_minimum_required(VERSION 3.28)

project(CollMPA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "src")
set(INCLUDE_DIR "include")

set(SOURCES
  "${SOURCE_DIR}/main.cpp"
  "${SOURCE_DIR}/Number.cpp"
  "${SOURCE_DIR}/SSA.cpp"
)

set(HEADERS
  "${INCLUDE_DIR}/Number.hpp"
  "${INCLUDE_DIR}/SSA.hpp"
)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

