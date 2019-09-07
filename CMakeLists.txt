#########
# CMAKE #
#########

cmake_minimum_required(VERSION 2.8)

project(watchdog)

## set compilation flags
set(CMAKE_C_FLAGS "-W -Wall -Werror -pedantic")

## set the target name and source
add_executable(
    watchdog
    src/main.c
<<<<<<< HEAD
=======
    src/watchdog.c
>>>>>>> f46d8b70a612d1463454a00897ef6fef26906462
)

