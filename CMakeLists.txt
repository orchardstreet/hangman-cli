cmake_minimum_required(VERSION 3.0)

project(hangman LANGUAGES C)

add_executable(hangman main.c)

install(TARGETS hangman RUNTIME DESTINATION bin)
