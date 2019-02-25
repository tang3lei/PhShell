cmake_minimum_required(VERSION 3.12)
project(MyShell)

set(CMAKE_CXX_STANDARD 14)

add_executable(cp practice/cp.c)
add_executable(ls practice/ls.c)
add_executable(sh practice/sh.c)
add_executable(pipe practice/pipe.c)
add_executable(smsh practice/smsh.h practice/smsh1.c practice/execute.c practice/splitline.c practice/process.c pch.h practice/controflow.c)
add_executable(phsh src/Phsh.h src/Phsh.cpp src/test.cpp src/Processer.cpp src/Processer.h src/Varlib.cpp src/Varlib.h)

target_link_libraries(phsh pthread)