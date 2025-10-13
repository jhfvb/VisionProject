# S.1 工程创建
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
PROJECT(VisionDLL)
set(CMAKE_CXX_STANDARD 17)

#  S.2 定义binary和library路径
set(ZZ_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "zhuzhao root ")
set(ZZ_DEBUG_BUILD_DIR "${ZZ_ROOT}/../bin" CACHE PATH "Debug    Build Dir")
#统一输出目录到../bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_LIBRARY_OUT_PUT_DIRECTOY_RELEASE ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${ZZ_DEBUG_BUILD_DIR})

#S.3寻找第三方库
FIND_PACKAGE(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found ! Please set OpenCV first.")
endif()

#S.4寻找Qt依赖路径（因VisionProcessor继承QObject，需要连接QtCore)
FIND_PACKAGE(Qt6 REQUIRED COMPONENTS Core Widgets)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found ! Please set Qt6 first.")
endif()

#S.5 创建视觉算法动态库
FILE(GLOB
    DLL_SRCS
    src/VisionProcessor.cpp
    include/VisionProcessor.h
)

#生成SHARED动态库
add_library(VisionDLL SHARED ${DLL_SRCS})
#添加宏导出定义
add_compile_definitions(ALGO_EXPORT)
#链接依赖库
TARGET_LINK_LIBRARIES(VisionDLL PRIVATE ${OpenCV_LIBS}
    Qt6::Core
    Qt6::Widgets)

    #S.6
    target_include_directories(VisionDLL
    PUBLIC ${ZZ_ROOT}/include#对外暴露的头文件路径
    PRIVATE ${OpenCV_INCLUDE_DIRS}#OpenCV内部头文件
    PRIVATE ${QT6_INCLUDE_DIRS}#Qt内部头文件
    )

    #S.7 创建动态库示例程序
FILE(GLOB
    EXAMPLE_SRCS
  example/ExampleMain.cpp
)

#生成示例可执行文件
add_executable(VisionDLLExample ${EXAMPLE_SRCS})
#链接示例程序依赖
target_link_libraries(VisionDLLExample 
    ${OpenCV_LIBS}
    VisionDLL
    Qt6::Core
    Qt6::Widgets
)