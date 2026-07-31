cmake_minimum_required(VERSION 3.20)

set(BUILD_TESTING OFF CACHE BOOL "Disable tests")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY CACHE STRING
    "Skip linking during try_compile (cross-compiling)"
)

project(UCraft-printer C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ── Shared toolchain ───────────────────────────────────────────────────
include(${CMAKE_SOURCE_DIR}/toolchain-armeb-eabi.cmake)

# ── Common architecture flags ──────────────────────────────────────────
add_compile_options(
    -march=armv5t
    -mbig-endian
    -ffunction-sections
)
include_directories(${CMAKE_SOURCE_DIR})

# ── Sub-projects ───────────────────────────────────────────────────────
add_subdirectory(first_stage)
add_subdirectory(application)
