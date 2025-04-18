cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# 在 CMAKE_C_STANDARD 附近添加 C++ 标准设置
set(CMAKE_CXX_STANDARD 17)  # 或 14/11，根据需求选择
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# 启用microLIB
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -specs=nosys.specs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -specs=nosys.specs")


# 启用 Ninja 颜色支持
add_compile_options(-fdiagnostics-color=always)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME demo)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# 添加到工具链包含之后（确保覆盖可能存在的默认值）
set(CMAKE_SYSROOT "/opt/gcc-arm-none-eabi-10.3-2021.10/arm-none-eabi")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)  # 仅在sysroot中搜索头文件[4,6](@ref)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages 和c++
enable_language(C CXX ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# 从./Drivers/user/Src遍历的添加源文件，添加到USER_SOURCES变量中
file(GLOB USER_SOURCES 
    ./Drivers/User/Src/*.c
    ./Drivers/User/Src/*.cpp
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${USER_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ./Drivers/User/Inc/
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    stdc++
    nosys
)

# 添加生成 hex 文件的步骤
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex
    COMMENT "Generating hex file from elf"
)
