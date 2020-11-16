#########
# CMAKE #
#########

cmake_minimum_required(VERSION 2.8)

project(watchdog)

## set compilation flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Werror -pedantic")

## set the target name and source
add_executable(
    watchdog-manager
    src/main.c
    src/watchdog.c
)

# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)

# Set the location for executable installation
install(TARGETS watchdog DESTINATION ${BINDIR})