project(static_reflection_test VERSION 0.1.0 LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    static_reflection_test.cpp)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    GTest::gtest GTest::gmock)

add_test(AllTestsInMain ${PROJECT_NAME})