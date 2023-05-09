#cmake_minimum_required(VERSION 3.18...3.22)
cmake_minimum_required(VERSION 3.15...3.26)
project(elkai)

find_package(Python
  COMPONENTS Interpreter Development.Module REQUIRED)

include_directories(LKH-3.0.8/SRC/INCLUDE)
file(GLOB LKH_SRC "LKH-3.0.8/SRC/*.c")

python_add_library(_elkai MODULE elkai/_elkai.c ${LKH_SRC})
target_compile_definitions(_elkai PRIVATE TWO_LEVEL_TREE)
target_compile_definitions(_elkai PRIVATE PYTHON_ERR_HANDLE)

install(TARGETS _elkai LIBRARY DESTINATION elkai)
