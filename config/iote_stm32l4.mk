################################################################################
# IoT-Engine STM32L4 (Cortex-M4 / ARMv7-M)  —  toolchain & board mapping
################################################################################

GCC  ?= arm-none-eabi-gcc
AS   ?= arm-none-eabi-gcc
LINK ?= arm-none-eabi-gcc

CFLAGS ?= -mcpu=cortex-m4 -mthumb -ffreestanding \
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \
    -mfloat-abi=soft

ASFLAGS ?= -mcpu=cortex-m4 -mthumb -ffreestanding \
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP

LFLAGS ?= -mcpu=cortex-m4 -mthumb -ffreestanding \
    -nostartfiles \
    -O0 -g3 \
    -mfloat-abi=soft

LNKFILE ?= etc/linker/iote_stm32l4/tkernel_map.ld

# Directory mapping for sysdepend layers
BOARD_DIR := iote_stm32l4
CPU_DIR   := stm32l4
CORE_DIR  := armv7m
