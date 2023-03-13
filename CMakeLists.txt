cmake_minimum_required(VERSION 3.24)
project(lab_oc_3 C)

set(CMAKE_C_STANDARD 11)

#add_executable(lab_oc_3 test.c string_ops.c copy_file.c)
add_executable(lab_oc_3 main.c copy_file.c string_ops.c mem_ops.c)
