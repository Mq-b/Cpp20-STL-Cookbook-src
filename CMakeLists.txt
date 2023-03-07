cmake_minimum_required (VERSION 3.8)

project (Test1)

set(CMAKE_CXX_STANDARD 23)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 将源代码添加到此项目的可执行文件。
add_executable (Test1 "src/7.10使用文件输入初始化复杂结构体.cpp")