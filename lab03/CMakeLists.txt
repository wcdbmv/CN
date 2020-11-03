cmake_minimum_required(VERSION 3.5)
project(lab03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
	client
	const.hpp
	client.cpp
	utils.cpp
	utils.hpp
)

add_executable(
	server
	const.hpp
	http_server.cpp
	http_server.hpp
	server.cpp
	statistics.cpp
	statistics.hpp
	utils.cpp
	utils.hpp
)
