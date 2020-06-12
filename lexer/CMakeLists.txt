cmake_minimum_required(VERSION 3.16)
project(lexer)

set(CMAKE_CXX_STANDARD 14)

add_executable(lexer main.cpp Lexer.cpp Lexer.h TokenTypesList.h Token.h)