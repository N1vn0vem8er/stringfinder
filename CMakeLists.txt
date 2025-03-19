cmake_minimum_required(VERSION 3.0)

project(stringfinder LANGUAGES C)

add_executable(stringfinder main.c)

set_property(TARGET stringfinder PROPERTY C_STANDARD 23)

install(TARGETS stringfinder RUNTIME DESTINATION bin)
