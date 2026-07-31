if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    project(program_loader C ASM)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../toolchain-armeb-eabi.cmake)
    add_compile_options(-march=armv5t -mbig-endian -ffunction-sections)
endif()

# ── Linker script ──────────────────────────────────────────────────────
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.lds)

# ── First-stage-specific compile / link flags ───────────────────────────
add_compile_options(-fpic )

add_link_options(
    -fpic
    -nostdlib
    -T${LINKER_SCRIPT}
    -Wl,--gc-sections
    -Wl,-pie
    -Wl,--no-dynamic-linker
    -Wl,-Bsymbolic
    -Wl,--no-warn-rwx-segments
)

# ── Executable ──────────────────────────────────────────────────────────
add_executable(program_loader
    src/main.c
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    target_include_directories(program_loader PRIVATE ${CMAKE_SOURCE_DIR}/..)
else()
    target_include_directories(program_loader PRIVATE ${CMAKE_SOURCE_DIR})
endif()

set_target_properties(program_loader PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT}
    SUFFIX ".elf"
)

# ── Post-build: raw binary ─────────────────────────────────────────────
add_custom_command(TARGET program_loader POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:program_loader>
            ${CMAKE_BINARY_DIR}/program_loader.bin
            --set-section-flags .bss=alloc,load,contents
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:program_loader>
    COMMENT "Generating program_loader.bin"
)
