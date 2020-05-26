################################################################################
# micro T-Kernel 3.00.01  makefile
################################################################################

GCC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
SIZE := arm-none-eabi-size

CFLAGS := -mcpu=cortex-m3 -mthumb \
    -O0 -ffreestanding -g3 -std=gnu11 \
    -MMD -MP \

ASFLAGS := -mcpu=cortex-m3 -mthumb \
    -O0 -ffreestanding  -g3 -x assembler-with-cpp \
    -MMD -MP \

LFLAGS := -mcpu=cortex-m3 -mthumb \
    -O0 -ffreestanding -g3 -nostartfiles \

LNKFILE := "..\etc\linker\iote_rx231\tkernel_map.ld"

-include mtkernel_3/lib/libtm/sysdepend/iote_rx231/subdir.mk
-include mtkernel_3/lib/libtk/sysdepend/cpu/rx231/subdir.mk
-include mtkernel_3/lib/libtk/sysdepend/cpu/core/rxv2/subdir.mk
-include mtkernel_3/kernel/sysdepend/iote_rx231/subdir.mk
-include mtkernel_3/kernel/sysdepend/cpu/rx231/subdir.mk
-include mtkernel_3/kernel/sysdepend/cpu/core/rxv2/subdir.mk
