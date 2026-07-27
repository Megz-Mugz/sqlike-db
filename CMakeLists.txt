cmake_minimum_required(VERSION 3.20)

project(
    sqllike
    VERSION 0.1.0
    DESCRIPTION "A SQL-like database implemented in C++"
    LANGUAGES CXX
)

include(CTest)

add_library(sqllike_core
    src/ast.cpp
    src/typechecker.cpp
    src/code_generator.cpp
    src/create_statement.cpp
    src/database.cpp
    src/delete_statement.cpp
    src/executor.cpp
    src/insert_statement.cpp
    src/lexer.cpp
    src/parser.cpp
    src/schema.cpp
    src/select_statement.cpp
    src/update_statement.cpp
    src/schema.cpp
)

target_compile_features(sqllike_core PUBLIC cxx_std_23)
set_target_properties(sqllike_core PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(sqllike_core
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>"
)

if(MSVC)
    target_compile_options(sqllike_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(sqllike_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(sqllike src/main.cpp)
target_link_libraries(sqllike PRIVATE sqllike_core)

if(BUILD_TESTING)
    find_package(GTest REQUIRED)

    add_executable(sqllike_tests
        tests/create_table_test.cpp
        tests/delete_statement_test.cpp
        tests/drop_table_test.cpp
        tests/insert_statement_test.cpp
        tests/select_statement_test.cpp
        tests/update_statement_test.cpp
    )

    target_link_libraries(sqllike_tests
        PRIVATE
            sqllike_core
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(sqllike_tests)
endif()
