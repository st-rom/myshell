cmake_minimum_required(VERSION 3.9)
project(myshell)

set(CMAKE_CXX_STANDARD 11)

add_executable(myshell myshell.cpp mygrep.cpp mycp.cpp myrm.cpp mymkdir.cpp mymv.cpp mycat.cpp myls.cpp)
