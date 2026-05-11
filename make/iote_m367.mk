################################################################################
# IoT-Engine M367 (Cortex-M3 / ACM3)  —  toolchain & board mapping
################################################################################

GCC  ?= arm-none-eabi-gcc
AS   ?= arm-none-eabi-gcc
LINK ?= arm-none-eabi-gcc

CFLAGS ?= -mcpu=cortex-m3 -mthumb -ffreestanding \
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP

ASFLAGS ?= -mcpu=cortex-m3 -mthumb -ffreestanding \
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP

LFLAGS ?= -mcpu=cortex-m3 -mthumb -ffreestanding \
    -nostartfiles \
    -O0 -g3

LNKFILE ?= etc/linker/iote_m367/tkernel_map.ld

# Directory mapping for sysdepend layers
BOARD_DIR := iote_m367
CPU_DIR   := tx03_m367
CORE_DIR  := armv7m
