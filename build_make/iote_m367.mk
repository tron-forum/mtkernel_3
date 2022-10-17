################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

GCC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
LINK := arm-none-eabi-gcc

CFLAGS := -mcpu=cortex-m3 -mthumb -ffreestanding\
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \

ASFLAGS := -mcpu=cortex-m3 -mthumb -ffreestanding\
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP \

LFLAGS := -mcpu=cortex-m3 -mthumb -ffreestanding \
    -nostartfiles \
    -O0 -g3 \

LNKFILE := "../etc/linker/iote_m367/tkernel_map.ld"

include mtkernel_3/lib/libtm/sysdepend/iote_m367/subdir.mk
include mtkernel_3/lib/libtm/sysdepend/no_device/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/tx03_m367/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/subdir.mk
include mtkernel_3/kernel/sysdepend/iote_m367/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/tx03_m367/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/core/armv7m/subdir.mk
