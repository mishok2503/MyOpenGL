cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MyOpenGl)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Werror")

add_executable(render "src/main.cpp"
	      "TGA library/tgaimage.cpp" "TGA library/tgaimage.h"
	      "include/vector.h"
	      "include/matrix.h"
	      "src/model.cpp" "include/model.h")

target_include_directories(render PUBLIC "include" "src" "TGA library")
