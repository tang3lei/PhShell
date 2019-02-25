cmake_minimum_required(VERSION 3.12)
project(MyShell)

set(CMAKE_CXX_STANDARD 14)

add_executable(cp  cp.c)
add_executable(ls ls.c )
add_executable(sh sh.c)
add_executable(pipe pipe.c)
add_executable(smsh smsh.h smsh1.c execute.c splitline.c process.c pch.h controflow.c)
add_executable(phsh src/Phsh.h src/Phsh.cpp src/test.cpp src/Processer.cpp src/Processer.h src/Varlib.cpp src/Varlib.h)

target_link_libraries(phsh pthread)