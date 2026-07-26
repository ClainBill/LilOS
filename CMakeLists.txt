# ============================================================================
#  LilOS build configuration
# ============================================================================
#  CMake does NOT build your code. It's a generator: it reads this file and
#  writes the actual build files (Makefiles here) into a directory you choose.
#  So building is always two steps:
#
#      cmake -B build      <- configure: read this file, generate build files
#      cmake --build build <- build:     run the generated build files
#
#  Everything CMake produces goes in build/, which is gitignored. Nothing
#  generated ever lands next to your source. That's an "out-of-source build",
#  and it's why `rm -rf build` is always safe.
# ============================================================================


# --- Minimum CMake version ---------------------------------------------------
# Declares which CMake features this file is allowed to use, and which
# behavioural defaults apply. CMake changes its defaults between versions and
# uses this line to decide which era of behaviour to give you, so old projects
# keep working. 3.20 is a modern baseline; you have 4.4 installed.
cmake_minimum_required(VERSION 3.20)


# --- Project declaration -----------------------------------------------------
# Names the project, sets its version, and declares which languages it uses.
# LANGUAGES CXX means C++ only - without it CMake also looks for a C compiler
# and complains if it can't find one.
project(LilOS
    VERSION 0.1.0
    DESCRIPTION "A micro OS in the spirit of the TI-83"
    LANGUAGES CXX
)


# --- C++ standard ------------------------------------------------------------
# 17 matches what the code already uses.
set(CMAKE_CXX_STANDARD 17)

# Without this, CMake treats the standard as a preference: if the compiler
# doesn't support C++17 it silently falls back to an older one and you get
# baffling errors. ON makes it a hard requirement instead.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilers add non-standard extensions (clang would use -std=gnu++17 rather
# than -std=c++17). Those extensions won't exist on an embedded toolchain, so
# turning them off now stops you accidentally depending on one.
set(CMAKE_CXX_EXTENSIONS OFF)


# --- Build type --------------------------------------------------------------
# CMake's build type decides the optimisation and debug flags. If you don't set
# one, it defaults to EMPTY - meaning no -O and, importantly, no -g. The binary
# still runs, but it carries no debug symbols, so breakpoints and variable
# inspection don't work properly.
#
# The if() guard means "only set this if the user didn't ask for something else",
# so `cmake -B build -DCMAKE_BUILD_TYPE=Release` still overrides it.
#
#   Debug           -g -O0     symbols, no optimisation. Default while developing.
#   Release         -O3        fast, no symbols.
#   RelWithDebInfo  -O2 -g     both. Useful for profiling.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


# --- compile_commands.json ---------------------------------------------------
# Writes build/compile_commands.json: a list of the exact compiler command used
# for every source file. Editors and tools read it to know your include paths
# and flags, so IntelliSense sees precisely what the compiler sees.
# This is what stops "it compiles fine but VS Code underlines it in red".
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# --- Dependencies ------------------------------------------------------------
# Looks for SDL2 on this machine. REQUIRED means stop with a clear error now if
# it's missing, rather than failing with confusing linker errors later.
#
# Homebrew ships SDL2Config.cmake, so this finds SDL2 wherever it's installed
# instead of us hardcoding /opt/homebrew - which would break on any other
# machine, including your own if you ever move to a different Mac.
find_package(SDL2 REQUIRED)


# --- Source files ------------------------------------------------------------
# Listed explicitly, on purpose. CMake can glob (file(GLOB ...)), but globs are
# evaluated at configure time only: add a new .cpp and the build won't know it
# exists until something else triggers a reconfigure. That produces a genuinely
# baffling bug - your new code appears not to run because it was never compiled.
#
# Listing files by hand means adding one is a visible line in your diff, which
# a reviewer can see. Cost: one line per file. Worth it.
set(LILOS_SOURCES
    src/main.cpp
    src/graphics.cpp
    src/text.cpp
    src/fonts/TI83Font5x7.cpp
)


# --- The executable ----------------------------------------------------------
# A "target" is one thing CMake knows how to build. This declares a target named
# lilos, built from those sources. The binary lands at build/lilos.
add_executable(lilos ${LILOS_SOURCES})


# --- Include directories -----------------------------------------------------
# Adds src/ as a root for #include "..." lookups, so any file can write
#     #include "fonts/font.h"
# rather than counting directory levels with "../fonts/font.h". Relative paths
# with ../ break the moment a file moves; paths from a fixed root don't.
#
# PRIVATE means "this target needs it, things that link against it don't".
# The alternative, PUBLIC, would also apply the setting to anything depending on
# this target. For a standalone executable nothing depends on it, so PRIVATE.
# When you split into a library + platform layer, that distinction starts to matter.
target_include_directories(lilos PRIVATE src)


# --- Warnings ----------------------------------------------------------------
# The tree is currently warning-clean. Keep it that way - warnings in C++ are
# usually the compiler telling you about a real bug it isn't allowed to reject.
#
#   -Wall      the common, high-value warnings (despite the name, not all of them)
#   -Wextra    a second batch that -Wall leaves out
#   -Wpedantic complain about anything not strictly standard C++
#
# Worth knowing about for later: -Werror turns warnings into hard errors. Good
# in CI. Painful while you're mid-refactor, so not on yet.
target_compile_options(lilos PRIVATE -Wall -Wextra -Wpedantic)


# --- Linking -----------------------------------------------------------------
# SDL2::SDL2 is an "imported target": it carries SDL2's include paths, library
# path and link flags as one bundle, so this single line replaces the
# -I/opt/homebrew/... -L/opt/homebrew/... -lSDL2 trio from the old build command.
target_link_libraries(lilos PRIVATE SDL2::SDL2)


# ============================================================================
#  Later: enforcing the src/ <-> platform/ boundary
# ============================================================================
#  Right now this is one target, so nothing stops src/ from including SDL -
#  and main.cpp still does.
#
#  Once main.cpp's SDL half moves to platform/, split this into two targets:
#
#      add_library(lilos_core STATIC src/graphics.cpp src/text.cpp ...)
#      target_include_directories(lilos_core PUBLIC src)
#      # note: no SDL2 linked here, and no SDL2 include path
#
#      add_executable(lilos platform/sdl/main.cpp platform/sdl/display.cpp)
#      target_link_libraries(lilos PRIVATE lilos_core SDL2::SDL2)
#
#  Because lilos_core never sees SDL2's include path, an #include <SDL2/SDL.h>
#  inside src/ stops compiling. The boundary goes from a rule you have to
#  remember to one the build enforces for you.
# ============================================================================
