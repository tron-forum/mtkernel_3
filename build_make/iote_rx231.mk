################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

GCC := rx-elf-gcc
AS := rx-elf-gcc
LINK := rx-elf-gcc

CFLAGS := -mcpu=rx230 -misa=v2 -mlittle-endian-data \
    -O0 -ffunction-sections -fdata-sections -g2 \
    -MMD -MP \

ASFLAGS := -mcpu=rx230 -misa=v2 -mlittle-endian-data \
    -O0 -ffunction-sections -fdata-sections -g2 \
    -x assembler-with-cpp -Wa,--gdwarf2 \
    -MMD -MP \

LFLAGS := -mcpu=rx230 -misa=v2 -mlittle-endian-data \
    -O0 -ffunction-sections -fdata-sections -g2 \
    -nostartfiles -nostdlib

LNKFILE := "..\etc\linker\iote_rx231\tkernel_map.ld"

include mtkernel_3/lib/libtm/sysdepend/iote_rx231/subdir.mk
include mtkernel_3/lib/libtm/sysdepend/no_device/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/rx231/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/core/rxv2/subdir.mk
include mtkernel_3/kernel/sysdepend/iote_rx231/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/rx231/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/core/rxv2/subdir.mk
