cmake_minimum_required(VERSION 3.5)
project(lab02 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(
	-Werror

	-Wall
	-Wextra
	-Wpedantic

	-Wconversion
)

set(COMMON socket.h socket.c my_lltoa.h my_lltoa.c)
add_executable(server server.c ${COMMON})
add_executable(client client.c ${COMMON})
