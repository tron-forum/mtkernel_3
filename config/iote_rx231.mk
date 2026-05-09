################################################################################
# IoT-Engine RX231 (RXv2)  —  toolchain & board mapping
################################################################################

GCC  ?= rx-elf-gcc
AS   ?= rx-elf-gcc
LINK ?= rx-elf-gcc

CFLAGS ?= -mcpu=rx230 -misa=v2 -mlittle-endian-data \
    -O0 -ffunction-sections -fdata-sections -g2 \
    -MMD -MP

ASFLAGS ?= -mcpu=rx230 -misa=v2 -mlittle-endian-data \
    -O0 -ffunction-sections -fdata-sections -g2 \
    -x assembler-with-cpp -Wa,--gdwarf2 \
    -MMD -MP

LFLAGS ?= -mcpu=rx230 -misa=v2 -mlittle-endian-data \
    -O0 -ffunction-sections -fdata-sections -g2 \
    -nostartfiles -nostdlib

LNKFILE ?= etc/linker/iote_rx231/tkernel_map.ld

# Directory mapping for sysdepend layers
BOARD_DIR := iote_rx231
CPU_DIR   := rx231
CORE_DIR  := rxv2
