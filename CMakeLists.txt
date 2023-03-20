cmake_minimum_required(VERSION 3.11.0)
project(elkai)

# set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PythonExtensions REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/LKH-3.0.8/SRC/INCLUDE)
file(GLOB LKH_SRC "LKH-3.0.8/SRC/*.c")

# add_library(LKH ${LKH_SRC})
# target_compile_definitions(LKH PRIVATE TWO_LEVEL_TREE)
# target_compile_options(LKH PRIVATE -flto -fcommon)

add_library(_elkai MODULE elkai/_elkai.c ${LKH_SRC})
python_extension_module(_elkai)
# target_link_libraries(_elkai LKH)

install(TARGETS _elkai LIBRARY DESTINATION elkai)
