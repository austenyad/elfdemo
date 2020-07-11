## Cmake 生成不同平台下的 makefile 文件 --> 一个 make 命令就可以生成对应平台下的可执行文件。
cmake_minimum_required(VERSION 3.16)
project(elfdemo)

set(CMAKE_CXX_STANDARD 14)
## 发现一个目录下所有源代码文件并将列表储存在一个变量总
AUX_SOURCE_DIRECTORY(./ DIR_SOURCE)
add_executable(elfdemo ${DIR_SOURCE})

## add_library(libname STATIC ${DIR_SOURCE}) 添加静态库
## add_library(libname SHARED ${DIR_SOURCE}) 添加动态库
## add_subdirectory(./libname) 向当前工程添加 存放在源文件的子目录。并可以指向中间二进制和目标二级制的存放位置
#target_link_libraries() 为target 添加需要的链接的共享库
