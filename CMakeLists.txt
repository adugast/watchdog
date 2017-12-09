#########
# CMAKE #
#########

cmake_minimum_required(VERSION 3.7.2)

project(watchdog)

## set compilation flags
set(CMAKE_C_FLAGS "-W -Wall -Werror -pedantic")

## set the target name and source
add_executable(
    watchdog
    src/main.c
)

