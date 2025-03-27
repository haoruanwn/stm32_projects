cmake_minimum_required(VERSION 3.12)
project(lvgl LANGUAGES C)

# [1] 递归收集所有源文件（排除特定平台文件）
file(GLOB_RECURSE LVGL_SOURCES
    "src/*.c"
    "src/*.h"
)

# [2] 过滤不需要的源文件（根据需求调整）
# list(FILTER LVGL_SOURCES EXCLUDE REGEX ".*/examples/.*")
# list(FILTER LVGL_SOURCES EXCLUDE REGEX ".*/tests/.*")

# [3] 创建LVGL静态库目标
add_library(lvgl STATIC ${LVGL_SOURCES})

# [4] 指定公共头文件路径
# 不用添加所有的头文件，只需要添加src目录即可，子目录中的源文件基本都是用的相对路径
target_include_directories(lvgl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# # [5] 添加编译器选项（适配STM32）
# target_compile_options(lvgl PRIVATE
#     -Wno-unused-parameter
#     -Wno-sign-compare
# )

# # [6] 配置LVGL特性（通过定义宏）
# target_compile_definitions(lvgl PUBLIC
#     LV_CONF_INCLUDE_SIMPLE=1
#     LV_LVGL_H_INCLUDE_SIMPLE=1
# )