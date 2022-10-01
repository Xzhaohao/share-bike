cmake_minimum_required(VERSION 3.16)

# 搜集所有在指定路径下的源文件的文件名，将输出结果列表存储在指定变量中
aux_source_directory(. SOURCE_COMMON_FILES)

# 构建库供其他模块使用
add_library(common ${SOURCE_COMMON_FILES})

# 用来显式的定义变量
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -Wall -m64 -pipe -std=c++0x -lrt -Wno-reorder -Wdeprecated-declarations")

# 将指定目录添加到编译器的头文件搜索路径之下
include_directories(../../third/include)

# 将指定目录添加到需要链接的库文件目录之下
link_directories(../../third/lib/iniparser)
link_directories(../../third/lib/log4cpp)

# 将目标文件与库文件进行链接
target_link_libraries(common iniparser)
target_link_libraries(common log4cpp)
target_link_libraries(common pthread)
target_link_libraries(common dl)
