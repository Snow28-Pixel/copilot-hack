cmake_minimum_required(VERSION 3.16)
project(kaoyan_data_structures C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(ds STATIC
    src/linear.c
    src/stack_queue.c
    src/string.c
    src/tree.c
    src/graph.c
    src/search_sort.c
)
target_include_directories(ds PUBLIC include)

if(MSVC)
    target_compile_options(ds PRIVATE /W4 /utf-8)
else()
    target_compile_options(ds PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(test_ds tests/test_ds.c)
target_link_libraries(test_ds PRIVATE ds)
if(MSVC)
    target_compile_options(test_ds PRIVATE /W4 /utf-8)
else()
    target_compile_options(test_ds PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME data_structures COMMAND test_ds)
