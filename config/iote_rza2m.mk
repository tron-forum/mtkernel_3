################################################################################
# IoT-Engine RZA2M (Cortex-A9 / ARMv7-A)  —  toolchain & board mapping
################################################################################

GCC  ?= arm-none-eabi-gcc
AS   ?= arm-none-eabi-gcc
LINK ?= arm-none-eabi-gcc

CFLAGS ?= -mcpu=cortex-a9 -mthumb -ffreestanding \
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \
    -mfloat-abi=soft

ASFLAGS ?= -mcpu=cortex-a9 -mthumb -ffreestanding \
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP

LFLAGS ?= -mcpu=cortex-a9 -mthumb -ffreestanding \
    -nostartfiles \
    -O0 -g3 \
    -mfloat-abi=soft

LNKFILE ?= etc/linker/iote_rza2m/tkernel_map.ld

# Directory mapping for sysdepend layers
BOARD_DIR := iote_rza2m
CPU_DIR   := rza2m
CORE_DIR  := armv7a
