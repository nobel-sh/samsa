cmake_minimum_required(VERSION 3.16)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

project(inline_asm VERSION 0.1)

add_executable(inline_asm src/main.cc src/token.cc src/lexer.cc src/parser.cc
    src/ast.cc src/validation.cc src/translation.cc
)

target_include_directories(inline_asm PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(inline_asm PUBLIC ${PROJECT_SOURCE_DIR}/src)
