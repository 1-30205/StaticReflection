cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_TOOLCHAIN_FILE "~/.vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")  # 使用 vcpkg 工具链

set(CMAKE_CXX_STANDARD 17)           # 将 C++ 标准设置为 C++ 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # C++ 17 是强制要求，不会衰退至低版本
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁止使用编译器特有扩展

project(static_reflection VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_subdirectory(test)

add_executable(${PROJECT_NAME}
    src/main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/include)