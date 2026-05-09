################################################################################
# IoT-Engine RX231 (RXv2)
################################################################################
set(BOARD_DIR   iote_rx231)
set(CPU_DIR     rx231)
set(CORE_DIR    rxv2)
set(BOARD_DEF   _IOTE_RX231_)

include(${CMAKE_CURRENT_LIST_DIR}/../rx-elf.cmake)

set(BOARD_C_FLAGS   -mcpu=rx230 -misa=v2 -mlittle-endian-data -ffunction-sections -fdata-sections)
set(BOARD_ASM_FLAGS -mcpu=rx230 -misa=v2 -mlittle-endian-data -ffunction-sections -fdata-sections)
set(BOARD_LINK_FLAGS -mcpu=rx230 -misa=v2 -mlittle-endian-data -ffunction-sections -fdata-sections -nostartfiles -nostdlib)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/../../etc/linker/iote_rx231/tkernel_map.ld)
