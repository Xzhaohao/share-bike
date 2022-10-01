cmake_minimum_required(VERSION 3.16)

project(share_bike)

include_directories(../third/include)
include_directories(./common)

link_directories(../third/lib/iniparser)
link_directories(../third/lib/log4cpp)
link_directories(./common)

# 内置变量：CMAKE_SOURCE_DIR 定义了顶级CMakeLists.txt 所在的文件夹;
#         PROJECT_SOURCE_DIR 定义了包含最近的project()命令的CMakeLists.txt 所在的文件夹
aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_FILES)

# 使用给定的源文件，为工程引入一个可执行文件
add_executable(share_bike ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -Wall -m64 -pipe -std=c++0x -lrt -Wno-reorder -Wdeprecated-declarations -fpermissive")

target_link_libraries(share_bike iniparser)
target_link_libraries(share_bike log4cpp)
target_link_libraries(share_bike common)

# 增加子目录
add_subdirectory(common)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
install(TARGETS share_bike DESTINATION bin)
