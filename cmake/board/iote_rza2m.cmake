################################################################################
# IoT-Engine RZA2M (Cortex-A9 / ARMv7-A)
################################################################################
set(BOARD_DIR   iote_rza2m)
set(CPU_DIR     rza2m)
set(CORE_DIR    armv7a)
set(BOARD_DEF   _IOTE_RZA2M_)

include(${CMAKE_CURRENT_LIST_DIR}/../arm-none-eabi.cmake)

set(BOARD_C_FLAGS   -mcpu=cortex-a9 -mthumb -mfloat-abi=soft -ffreestanding)
set(BOARD_ASM_FLAGS -mcpu=cortex-a9 -mthumb -ffreestanding)
set(BOARD_LINK_FLAGS -mcpu=cortex-a9 -mthumb -mfloat-abi=soft -ffreestanding -nostartfiles)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/../../etc/linker/iote_rza2m/tkernel_map.ld)
