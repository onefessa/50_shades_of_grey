cmake_minimum_required(VERSION 3.17.2)
project("XRay")

set (CSTRING_SRC "main.cpp")
set (CSTRING_HDR "geometry.hpp" "data_processing.hpp" "fun.hpp" "autotest.hpp" "my_structs.hpp" "CImg.h")

add_executable("${PROJECT_NAME}" "${CSTRING_SRC}" "${CSTRING_HDR}")

install (TARGETS "${PROJECT_NAME}" DESTINATION bin)	#$(CMAKE_INSTALL_PREFIX)/bin
install (FILES "${CSTRING_SRC}" DESTINATION src)  	#$(CMAKE_INSTALL_PREFIX)/src
install (FILES "${CSTRING_HDR}" DESTINATION hdr)  	#$(CMAKE_INSTALL_PREFIX)/hdr

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -Og")