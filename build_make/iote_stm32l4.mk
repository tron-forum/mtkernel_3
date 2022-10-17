################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

GCC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
LINK := arm-none-eabi-gcc

CFLAGS := -mcpu=cortex-m4 -mthumb -ffreestanding\
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \
    -mfloat-abi=soft \

ASFLAGS := -mcpu=cortex-m4 -mthumb -ffreestanding\
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP \

LFLAGS := -mcpu=cortex-m4 -mthumb -ffreestanding \
    -nostartfiles \
    -O0 -g3 \
    -mfloat-abi=soft \

LNKFILE := "../etc/linker/iote_stm32l4/tkernel_map.ld"

include mtkernel_3/lib/libtm/sysdepend/iote_stm32l4/subdir.mk
include mtkernel_3/lib/libtm/sysdepend/no_device/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/stm32l4/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/subdir.mk
include mtkernel_3/kernel/sysdepend/iote_stm32l4/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/stm32l4/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/core/armv7m/subdir.mk
