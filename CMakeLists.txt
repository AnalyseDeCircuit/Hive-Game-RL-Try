cmake_minimum_required(VERSION 3.10)
project(HiveGameAI)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -fno-omit-frame-pointer")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
endif()

# 包含目录
include_directories(include)

# 查找源文件
set(SRC_FILES 
    src/AIPlayer.cpp
    src/Board.cpp
    src/Error.cpp
    src/Game.cpp
    src/Piece.cpp
    src/Player.cpp
)

# 显示找到的源文件
message(STATUS "SRC_FILES: ${SRC_FILES}")

# 创建可执行文件
add_executable(HiveGameAI ${SRC_FILES} src/main_ai.cpp)

# 设置输出目录
set_target_properties(HiveGameAI
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# 打印构建信息
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Project will build: HiveGameAI")

