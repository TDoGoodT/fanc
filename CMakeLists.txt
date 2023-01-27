project(analyzer)
# add the executable
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CPPFLAGS -I/usr/include)

add_executable(analyzer analyzer.tab.cpp analyzer.tab.hpp lex.yy.cpp symbol_table_manager.cpp symbol_table_manager.hpp symbol_table.cpp symbol_table.hpp validations.cpp validations.hpp types.cpp types.hpp hw3_output.cpp hw3_output.hpp actions.cpp actions.hpp store.cpp store.hpp bp.hpp bp.cpp visitor.hpp visitor.cpp markers.hpp markers.cpp debug.hpp debug.cpp rules.hpp rules.hpp)
