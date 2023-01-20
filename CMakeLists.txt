cmake_minimum_required (VERSION 3.8)

project (Test1)

set(CMAKE_CXX_STANDARD 23)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 将源代码添加到此项目的可执行文件。
add_executable (Test1 "src/3.4常数时间内从未排序的向量中删除项.cpp")