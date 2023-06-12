cmake_minimum_required(VERSION 3.25)
project(fanc_analyzer)

set(CMAKE_CXX_STANDARD 14)
add_custom_target(
        flex_bison ALL
        DEPENDS scanner.lex
        DEPENDS parser.ypp
        COMMAND /usr/bin/flex ../scanner.lex
        COMMAND mv lex.yy.c scanner.cpp
        COMMAND /usr/bin/bison -d ../parser.ypp
)


include_directories(.)

add_executable(fanc_analyzer
        nodes.cpp
        nodes.hpp
        symbol_table.hpp
        tokens.hpp
        visitor_code_gen.cpp
        visitor_symbol_table.cpp
        visitor_validation.cpp
        visitor_types.cpp
        visitor_echo.cpp
        visitor_types.cpp
        visitor_scope.cpp
        scanner.cpp
        parser.tab.cpp
        parser.tab.hpp
        bp.cpp bp.hpp
        hw3_output.cpp
        hw3_output.hpp
        visitor.hpp parser.ypp)

add_dependencies(fanc_analyzer flex_bison)

add_custom_command(
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target flex_bison
        COMMENT "Running bison and flex"
        OUTPUT parser.tab.cpp parser.tab.hpp scanner.cpp
)