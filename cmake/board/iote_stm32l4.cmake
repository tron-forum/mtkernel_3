################################################################################
# IoT-Engine STM32L4 (Cortex-M4 / ARMv7-M)
################################################################################
set(BOARD_DIR   iote_stm32l4)
set(CPU_DIR     stm32l4)
set(CORE_DIR    armv7m)
set(BOARD_DEF   _IOTE_STM32L4_)

include(${CMAKE_CURRENT_LIST_DIR}/../arm-none-eabi.cmake)

set(BOARD_C_FLAGS   -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -ffreestanding)
set(BOARD_ASM_FLAGS -mcpu=cortex-m4 -mthumb -ffreestanding)
set(BOARD_LINK_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -ffreestanding -nostartfiles)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/../../etc/linker/iote_stm32l4/tkernel_map.ld)
