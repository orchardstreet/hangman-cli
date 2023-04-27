cmake_minimum_required(VERSION 3.0)

project(hangman LANGUAGES C)

add_executable(hangman hangman.c readline_custom.c board_display.c load_words.c)

install(TARGETS hangman RUNTIME DESTINATION bin)
