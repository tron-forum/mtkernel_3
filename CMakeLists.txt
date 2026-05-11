################################################################################
# micro T-Kernel 3 — CMake build system
#
# Usage:
#   cmake -B build/iote_stm32l4 -DBOARD=iote_stm32l4
#   cmake --build build/iote_stm32l4
#
# Or with a preset (if CMakePresets.json is available):
#   cmake --preset iote_stm32l4
#   cmake --build --preset iote_stm32l4
################################################################################
cmake_minimum_required(VERSION 3.20)

# BOARD must be set before project()
if(NOT DEFINED BOARD)
    set(BOARD "iote_stm32l4" CACHE STRING "Target board")
endif()

# Load board definition (sets BOARD_DIR, CPU_DIR, CORE_DIR, flags, toolchain)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/board/${BOARD}.cmake)

project(mtkernel_3
    LANGUAGES C ASM
    VERSION 3.0.7
)

# ── Generate autoconf.h from Kconfig ─────────────────────────────────────────
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(AUTOCONF_H ${CMAKE_CURRENT_BINARY_DIR}/autoconf.h)
set(CONFIG_H   ${CMAKE_CURRENT_BINARY_DIR}/config.h)
set(KCONFIG_CONF "" CACHE FILEPATH "Optional .conf file for Kconfig overrides")

file(WRITE ${CONFIG_H}
    "/* Auto-generated wrapper */\n"
    "#include \"autoconf.h\"\n"
)

add_custom_command(
    OUTPUT  ${AUTOCONF_H}
    COMMAND ${CMAKE_COMMAND} -E env
        "CONFIG_=" "srctree=${CMAKE_CURRENT_SOURCE_DIR}"
        ${Python3_EXECUTABLE} -c
        "import kconfiglib,os$<SEMICOLON>os.chdir('${CMAKE_CURRENT_SOURCE_DIR}')$<SEMICOLON>k=kconfiglib.Kconfig('Kconfig')$<SEMICOLON>k.load_allconfig('${KCONFIG_CONF}')$<SEMICOLON>k.write_autoconf('${AUTOCONF_H}')"
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig
    COMMENT "Generating autoconf.h from Kconfig"
    VERBATIM
)
add_custom_target(autoconf_h DEPENDS ${AUTOCONF_H})

# ── Compile definitions ──────────────────────────────────────────────────────
add_compile_definitions(${BOARD_DEF})

# ── Common include paths ─────────────────────────────────────────────────────
set(MTKERNEL_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/knlinc
)

# ── Helper: collect sources from a directory ─────────────────────────────────
function(mtk_collect_sources dir)
    file(GLOB _c_srcs "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.c")
    file(GLOB _s_srcs "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.S")
    target_sources(mtkernel_3 PRIVATE ${_c_srcs} ${_s_srcs})
endfunction()

# ── Main target ──────────────────────────────────────────────────────────────
add_executable(mtkernel_3)
add_dependencies(mtkernel_3 autoconf_h)

target_include_directories(mtkernel_3 PRIVATE ${MTKERNEL_INCLUDE_DIRS})

# Apply board-specific flags
target_compile_options(mtkernel_3 PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${BOARD_C_FLAGS} -std=gnu11 -O0 -g3>
    $<$<COMPILE_LANGUAGE:ASM>:${BOARD_ASM_FLAGS} -x assembler-with-cpp -O0 -g3>
)

target_link_options(mtkernel_3 PRIVATE
    ${BOARD_LINK_FLAGS}
    -O0 -g3
    -T ${LINKER_SCRIPT}
    "LINKER:-Map,$<TARGET_FILE_DIR:mtkernel_3>/mtkernel_3.map"
)

set_target_properties(mtkernel_3 PROPERTIES
    SUFFIX ".elf"
    LINK_DEPENDS "${LINKER_SCRIPT}"
)

# ── Kernel core ──────────────────────────────────────────────────────────────
mtk_collect_sources(kernel/tkernel)
mtk_collect_sources(kernel/sysinit)
mtk_collect_sources(kernel/inittask)
mtk_collect_sources(kernel/tstdlib)
mtk_collect_sources(kernel/usermain)

# Kernel sysdepend layers
mtk_collect_sources(kernel/sysdepend/${BOARD_DIR})
mtk_collect_sources(kernel/sysdepend/cpu/${CPU_DIR})
mtk_collect_sources(kernel/sysdepend/cpu/core/${CORE_DIR})

# ── Libraries ────────────────────────────────────────────────────────────────
# libtk
mtk_collect_sources(lib/libtk)
mtk_collect_sources(lib/libtk/sysdepend/cpu/${CPU_DIR})
mtk_collect_sources(lib/libtk/sysdepend/cpu/core/${CORE_DIR})

# libtm
mtk_collect_sources(lib/libtm)
mtk_collect_sources(lib/libtm/sysdepend/${BOARD_DIR})
mtk_collect_sources(lib/libtm/sysdepend/no_device)

# ── Device drivers ───────────────────────────────────────────────────────────
mtk_collect_sources(device/common/drvif)

set(DEVICE_DRIVERS ser i2c adc)
foreach(drv IN LISTS DEVICE_DRIVERS)
    mtk_collect_sources(device/${drv})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/device/${drv}/sysdepend/${CPU_DIR}")
        mtk_collect_sources(device/${drv}/sysdepend/${CPU_DIR})
    endif()
endforeach()
