# S.1 工程创建（无Qt依赖，仅依赖C++标准库和OpenCV）
cmake_minimum_required(VERSION 3.22)
project(VisionDLL)
set(CMAKE_CXX_STANDARD 11)  # 使用C++11标准（兼容大部分环境）

# S.2 定义输出目录（统一到../bin，与你的工程风格一致）
set(ZZ_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Project root")
set(ZZ_DEBUG_BUILD_DIR "${ZZ_ROOT}/../bin" CACHE PATH "Output directory")
# 配置所有类型的输出目录（动态库、静态库、可执行文件）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ZZ_DEBUG_BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ZZ_DEBUG_BUILD_DIR})

# S.3 寻找OpenCV依赖（必须，动态库核心依赖）
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found! Please install OpenCV (4.5+ recommended).")
endif()

# S.4 收集动态库源文件（.h和.cpp）
file(GLOB 
    DLL_SRCS
    src/VisionProcessor.cpp
    include/VisionProcessor.h
)

# S.5 生成动态库（SHARED类型，与你的PhotometricStereoDLL风格一致）
add_library(VisionDLL SHARED ${DLL_SRCS})
# 添加导出宏定义（与头文件中的ALGO_EXPORT对应）
add_compile_definitions(ALGO_EXPORT)
# 链接OpenCV库（动态库仅依赖OpenCV，无Qt）
target_link_libraries(VisionDLL PRIVATE ${OpenCV_LIBS})

# S.6 配置头文件目录（供动态库内部编译和外部调用）
target_include_directories(VisionDLL
    PUBLIC ${ZZ_ROOT}/include          # 对外暴露的头文件路径
    PRIVATE ${OpenCV_INCLUDE_DIRS}     # OpenCV内部头文件
)

# S.7 收集示例程序源文件
file(GLOB
    EXAMPLE_SRCS
    example/ExampleMain.cpp
)

# S.8 生成示例可执行文件
add_executable(VisionExample ${EXAMPLE_SRCS})
# 链接示例程序依赖（OpenCV + 动态库VisionDLL）
target_link_libraries(VisionExample PRIVATE ${OpenCV_LIBS} VisionDLL)