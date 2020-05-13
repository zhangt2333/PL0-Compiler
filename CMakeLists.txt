cmake_minimum_required(VERSION 3.8)
project(PL0_Compiler)

set(CMAKE_CXX_STANDARD 14)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif()

set(SOURCE_FILES
        src/code.h
        src/exceptionHandler.h
        src/lexer.cpp
        src/lexer.h
        src/parser.cpp
        src/parser.h
        src/symbol.h
        src/symbolTable.cpp
        src/symbolTable.h
        src/symbolType.h
        src/syntaxTree.cpp
        src/syntaxTree.h
        src/utils.cpp
        src/utils.h
        )

add_executable(PL0_Compiler ${SOURCE_FILES} src/main.cpp)