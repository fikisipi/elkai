cmake_minimum_required(VERSION 3.11.0)
project(elkai)
find_package(PythonExtensions REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/LKH-2.0.9/SRC/INCLUDE)
file(GLOB LKH_SRC "LKH-2.0.9/SRC/*.c")
add_library(LKH ${LKH_SRC})
target_compile_definitions(LKH PRIVATE TWO_LEVEL_TREE)

add_library(_elkai MODULE elkai/_elkai.c)
python_extension_module(_elkai)
target_link_libraries(_elkai LKH)

install(TARGETS _elkai LIBRARY DESTINATION elkai)
