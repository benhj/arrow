# (c) 2019-present Ben Jones

cmake_minimum_required(VERSION 3.10) # Increased to 3.10 for SFML 3.x and modern CMake features like target_link_libraries for imported targets.
project(arrow CXX) # Explicitly specify CXX for C++ project.

# Option to enable/disable graphics building
option(BUILD_GRAPHICS "Build with graphics support" OFF)

# Global C++ standard setting (preferred over flags in COMP_FLAGS)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF) # Generally good practice to avoid compiler-specific extensions unless needed

find_package(Threads)

find_package(CURL QUIET)
if(CURL_FOUND)
    # Using add_definitions here, but will suggest target_compile_definitions later.
    # For now, keeping consistent with your working version's pattern for WITH_HEAVY.
    # The 'arrow' target will pick this up.
    add_definitions(-DWITH_HEAVY)
endif()

# Conditionally find SFML and define graphics sources
if(BUILD_GRAPHICS)
    message("Will try and build graphics support with SFML")
    # Using add_definitions here, but will suggest target_compile_definitions later.
    # For now, keeping consistent with your working version's pattern for WITH_GRAPHICS.
    # The 'arrow' target will pick this up.
    add_definitions(-DWITH_GRAPHICS)

    # --- Find SFML (Explicitly look for version 3, and use modern targets) ---
    set(SFML_DIR /opt/homebrew/lib/cmake/SFML CACHE PATH "Path to SFML 3.x CMake config directory")

    find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

    if(SFML_FOUND)
        message(STATUS "DEBUG: SFML found. Version: ${SFML_VERSION_MAJOR}.${SFML_VERSION_MINOR}")
    else()
        message(FATAL_ERROR "SFML 3.x not found! Please ensure it's installed and correctly configured by Homebrew.")
    endif()

    # Source files for graphics module
    file(GLOB_RECURSE graphics_srcs src/graphics/*.cpp src/graphics/*.hpp src/graphics/*.h)
endif()


# put all source code in one place for convenience
file(GLOB lexer_srcs src/lexer/*.cpp src/lexer/*.hpp)
file(GLOB parser_srcs src/parser/*.cpp src/parser/*.hpp)
file(GLOB_RECURSE expressions_srcs src/expressions/*.cpp src/expressions/*.hpp)
file(GLOB_RECURSE statements_srcs src/statements/*.cpp src/statements/*.hpp)
file(GLOB representation_srcs src/representation/*.cpp src/representation/*.hpp)
file(GLOB utility_srcs src/utility/*.cpp src/utility/*.hpp)
file(GLOB_RECURSE receivers_srcs src/receivers/*.cpp src/receivers/*.hpp)
file(GLOB_RECURSE builtin_srcs src/builtin/*.cpp src/builtin/*.hpp)


# annoyingly, lbs here aren't picked up automatically
# link_directories(/usr/local/lib) # Generally avoid link_directories; prefer target_link_libraries with full paths or imported targets

# =========================================================================
# STEP 1: DEFINE ALL LIBRARIES AND EXECUTABLES (add_library, add_executable)
# =========================================================================

# break above sub-folders into individual libraries
add_library(parser_lib ${parser_srcs})
add_library(lexer_lib ${lexer_srcs})
add_library(expressions_lib ${expressions_srcs})
add_library(statements_lib ${statements_srcs})
add_library(representation_lib ${representation_srcs})
add_library(utility_lib ${utility_srcs})
add_library(receivers_lib ${receivers_srcs})
add_library(builtin_lib ${builtin_srcs})

# Conditionally add graphics library
if(BUILD_GRAPHICS)
    add_library(graphics_lib ${graphics_srcs})
endif()

# optionally build http built-ins if CURL is found
if(CURL_FOUND)
    message("Found curl! Will try and build the networking builtins")
    file(GLOB_RECURSE heavy_srcs src/heavy/*.cpp src/heavy/*.hpp)
    add_library(heavy_lib ${heavy_srcs})
endif()

# build the interpretor
add_executable(arrow src/main.cpp)

# =========================================================================
# STEP 2: SET PROPERTIES FOR ALL DEFINED TARGETS (compile definitions/options, include directories)
# =========================================================================

# compile options. Cleaned up, -std=c++17 is now global.
set(COMP_FLAGS -O3 -ffast-math -funroll-loops -Wno-ctor-dtor-privacy -fno-pic -fPIE -Wall)

target_compile_options(parser_lib PRIVATE ${COMP_FLAGS})
target_compile_options(lexer_lib PRIVATE ${COMP_FLAGS})
target_compile_options(expressions_lib PRIVATE ${COMP_FLAGS})
target_compile_options(statements_lib PRIVATE ${COMP_FLAGS})
target_compile_options(representation_lib PRIVATE ${COMP_FLAGS})
target_compile_options(utility_lib PRIVATE ${COMP_FLAGS})
target_compile_options(receivers_lib PRIVATE ${COMP_FLAGS})
target_compile_options(builtin_lib PRIVATE ${COMP_FLAGS})
target_compile_options(arrow PRIVATE ${COMP_FLAGS})

# Apply compile options to optional libraries if they are built
if(BUILD_GRAPHICS)
    target_include_directories(graphics_lib PUBLIC /opt/homebrew/include/)
    target_compile_options(graphics_lib PRIVATE ${COMP_FLAGS})
endif()
if(CURL_FOUND)
    target_compile_options(heavy_lib PRIVATE ${COMP_FLAGS})
endif()

# ensure headers in the src folder are compiler-found
# Apply common include directory to all libraries and the executable
target_include_directories(arrow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(parser_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(lexer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(expressions_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(statements_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(representation_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(utility_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(receivers_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(builtin_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
if(BUILD_GRAPHICS)
    target_include_directories(graphics_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()
if(CURL_FOUND)
    target_include_directories(heavy_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()

# =========================================================================
# STEP 3: LINK TARGETS (target_link_libraries)
# =========================================================================

# Link common libraries
target_link_libraries(arrow
                      parser_lib
                      lexer_lib
                      expressions_lib
                      statements_lib
                      representation_lib
                      utility_lib
                      receivers_lib
                      builtin_lib
                      ${CMAKE_THREAD_LIBS_INIT})

# Conditionally link graphics libraries and their dependencies
if(BUILD_GRAPHICS)
    target_link_libraries(arrow graphics_lib)
    # Link SFML using its imported targets. This is the modern and robust way.
    target_link_libraries(arrow SFML::Graphics SFML::Window SFML::System)
endif()

# Link http built-ins if CURL is found
if(CURL_FOUND)
    target_link_libraries(arrow heavy_lib ${CURL_LIBRARIES})
endif()

# =========================================================================
# STEP 4: CUSTOM COMMANDS (add_custom_command, add_test etc.)
# =========================================================================

add_custom_command(TARGET arrow POST_BUILD
    COMMAND arrow ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.ar
    COMMENT "Running tests..."
)