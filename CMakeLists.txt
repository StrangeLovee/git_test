#CMake版本最小要求
cmake_minimum_required(VERSION 2.0.0)

#定义SRC变量
set(SRC Test.cpp)

project(RB_TREE)

#编译SRC变量所代表的源代码，生成可执行文件
add_executable(TEST_RB_TREE $(SRC))