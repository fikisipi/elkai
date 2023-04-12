cmake_minimum_required(VERSION 3.11.0)
project(elkai)

find_package(PythonExtensions REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/LKH-3.0.8/SRC/INCLUDE)
file(GLOB LKH_SRC "LKH-3.0.8/SRC/*.c")

add_library(_elkai MODULE elkai/_elkai.c ${LKH_SRC})
python_extension_module(_elkai)

install(TARGETS _elkai LIBRARY DESTINATION elkai)
