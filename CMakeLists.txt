# (c) 2019 Ben Jones

cmake_minimum_required (VERSION 2.6)
project (arrow)

# put all source code in one place for convenience
file(GLOB lexer src/lexer/*.cpp src/lexer/*.hpp)
file(GLOB parser src/parser/*.cpp src/parser/*.hpp)
file(GLOB_RECURSE expressions src/expressions/*.cpp src/expressions/*.hpp)
file(GLOB_RECURSE statements src/statements/*.cpp src/statements/*.hpp)
file(GLOB representation src/representation/*.cpp src/representation/*.hpp)
file(GLOB utility src/utility/*.cpp src/utility/*.hpp)
file(GLOB_RECURSE receivers src/receivers/*.cpp src/receivers/*.hpp)
file(GLOB_RECURSE builtin src/builtin/*.cpp src/builtin/*.hpp)

# ensure headers in the src folder are compiler-found
include_directories(src)

# annoyingly, lbs here aren't picked up automatically
link_directories(/usr/local/lib)

# break above sub-folders into individual libraries
add_library(parser_lib ${parser})
add_library(lexer_lib ${lexer})
add_library(expressions_lib ${expressions})
add_library(statements_lib ${statements})
add_library(representation_lib ${representation})
add_library(utility_lib ${utility})
add_library(receivers_lib ${receivers})
add_library(builtin_lib ${builtin})

# build the interpretor
add_executable(arrow src/main.cpp)
target_link_libraries(arrow
                      parser_lib
                      lexer_lib
                      expressions_lib
                      statements_lib
                      representation_lib
                      utility_lib
                      receivers_lib
                      builtin_lib)

# compile options. Lots of redundancy here. Can prob clean up.
set(COMP_FLAGS -std=c++17 -O3 -ffast-math -funroll-loops -Wno-ctor-dtor-privacy -fno-pic)
target_compile_options(parser_lib PUBLIC ${COMP_FLAGS})
target_compile_options(lexer_lib PUBLIC ${COMP_FLAGS})
target_compile_options(expressions_lib PUBLIC ${COMP_FLAGS})
target_compile_options(statements_lib PUBLIC ${COMP_FLAGS})
target_compile_options(representation_lib PUBLIC ${COMP_FLAGS})
target_compile_options(utility_lib PUBLIC ${COMP_FLAGS})
target_compile_options(receivers_lib PUBLIC ${COMP_FLAGS})
target_compile_options(builtin_lib PUBLIC ${COMP_FLAGS})
target_compile_options(arrow PUBLIC ${COMP_FLAGS})

add_custom_command(TARGET arrow POST_BUILD
    COMMAND arrow ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.ar
    COMMENT "Running tests..."
)