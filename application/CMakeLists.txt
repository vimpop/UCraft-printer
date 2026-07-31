if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    project(application C ASM)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)

    include(${CMAKE_CURRENT_SOURCE_DIR}/../toolchain-armeb-eabi.cmake)
    add_compile_options(-march=armv5t -mbig-endian -ffunction-sections)
endif()


set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.lds)

add_definitions(-DCUSTOM_WRAPPER -D__MBED__)
add_compile_options(-fpic -O3 )

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

add_executable(application
    src/entry.S
    src/syscalls.c
    src/app.c
    src/mbedtls_rng.c
    src/UCraft/src/blocks.c
    src/UCraft/src/c2s.c
    src/UCraft/src/encryption.c
    src/UCraft/src/game.c
    src/UCraft/src/https.c
    src/UCraft/src/log.c
    src/UCraft/src/player.c
    src/UCraft/src/s2c.c
    src/UCraft/src/socketio.c
    src/UCraft/src/storage.c
    src/UCraft/src/util.c
    src/UCraft/src/world.c
    src/UCraft/src/UCraft.c
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    target_include_directories(application PRIVATE ${CMAKE_SOURCE_DIR}/..)
else()
    target_include_directories(application PRIVATE ${CMAKE_SOURCE_DIR})
endif()

target_include_directories(application PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_include_directories(application PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/UCraft/include/UCraft)
target_include_directories(application PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/UCraft/include/3rdparty)

include(${CMAKE_CURRENT_SOURCE_DIR}/src/UCraft/3rdparty/cmake/zlib/zlib.cmake)
target_link_libraries(application PRIVATE zlib)

set(MBEDTLS_CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/mbedtls_custom_config.h" CACHE FILEPATH "Mbed TLS config file")
include(${CMAKE_CURRENT_SOURCE_DIR}/src/UCraft/3rdparty/cmake/mbedtls/mbedtls.cmake)
target_include_directories(mbedtls PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_include_directories(application PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/UCraft/3rdparty/mbedtls/include)
target_link_libraries(application PRIVATE mbedtls)


include(${CMAKE_CURRENT_SOURCE_DIR}/src/UCraft/3rdparty/cmake/lwjson/lwjson.cmake)
target_link_libraries(application PRIVATE lwjson)



set_target_properties(application PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT}
    SUFFIX ".elf"
)

add_custom_command(TARGET application POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:application>
            ${CMAKE_BINARY_DIR}/application.bin
            --set-section-flags .heap=alloc,load,contents
            --set-section-flags .tdata=alloc,load,contents
            --set-section-flags .tbss=alloc,load,contents
            --set-section-flags .bss=alloc,load,contents
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:application>
    COMMENT "Generating application.bin"
)
