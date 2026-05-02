# CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(json2csv)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置输出位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 头文件搜索路径（让编译器找到 cJSON/cJSON.h）
include_directories(${PROJECT_SOURCE_DIR})

# 生成可执行文件：json2csv
# 源文件：json2csv.c + cJSON.c
add_executable(${PROJECT_NAME}
    json2csv.c
    cJSON.c
)
