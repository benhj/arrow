# (c) 2019 Ben Jones

cmake_minimum_required (VERSION 2.6)
project (parser-dev)

# put all source code in one place for convenience
file(GLOB lexer src/lexer/*.cpp src/lexer/*.hpp)
file(GLOB parser src/parser/*.cpp src/parser/*.hpp)
file(GLOB expressions src/expressions/*.cpp src/expressions/*.hpp)
file(GLOB statements src/statements/*.cpp src/statements/*.hpp)
file(GLOB representation src/representation/*.cpp src/representation/*.hpp)
file(GLOB evaluator src/evaluator/*.cpp src/evaluator/*.hpp)

# ensure headers in the src folder are compiler-found
include_directories(src)

# annoyingly, lbs here aren't picked up automatically
link_directories(/usr/local/lib)

# break above sub-folders into individual libraries
add_library(parser_lib ${parser})
add_library(lexer_lib ${lexer})
add_library(expressions_lib ${expressions})
add_library(statements_lib ${statements})
add_library(evaluator_lib ${evaluator})
add_library(representation_lib ${representation})

# build the interpretor
add_executable(output src/main.cpp)
target_link_libraries(output
                      parser_lib
                      lexer_lib
                      expressions_lib
                      statements_lib
                      evaluator_lib
                      representation_lib)

# compile options. Lots of redundancy here. Can prob clean up.
set(COMP_FLAGS -std=c++17 -O3 -ffast-math -funroll-loops -Wno-ctor-dtor-privacy -fno-pic)
target_compile_options(parser_lib PUBLIC ${COMP_FLAGS})
target_compile_options(lexer_lib PUBLIC ${COMP_FLAGS})
target_compile_options(evaluator_lib PUBLIC ${COMP_FLAGS})
target_compile_options(expressions_lib PUBLIC ${COMP_FLAGS})
target_compile_options(statements_lib PUBLIC ${COMP_FLAGS})
target_compile_options(representation_lib PUBLIC ${COMP_FLAGS})
target_compile_options(output PUBLIC ${COMP_FLAGS})
