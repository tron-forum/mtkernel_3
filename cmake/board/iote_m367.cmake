################################################################################
# IoT-Engine M367 (Cortex-M3 / ACM3)
################################################################################
set(BOARD_DIR   iote_m367)
set(CPU_DIR     tx03_m367)
set(CORE_DIR    armv7m)
set(BOARD_DEF   _IOTE_M367_)

include(${CMAKE_CURRENT_LIST_DIR}/../arm-none-eabi.cmake)

set(BOARD_C_FLAGS   -mcpu=cortex-m3 -mthumb -ffreestanding)
set(BOARD_ASM_FLAGS -mcpu=cortex-m3 -mthumb -ffreestanding)
set(BOARD_LINK_FLAGS -mcpu=cortex-m3 -mthumb -ffreestanding -nostartfiles)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/../../etc/linker/iote_m367/tkernel_map.ld)
