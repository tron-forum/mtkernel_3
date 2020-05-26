################################################################################
# micro T-Kernel 3.00.01  makefile
################################################################################

EXE_FILE := mtkernel_3
TARGET := _IOTE_M367_

RM := rm -rf

INCPATH = -I"..\include" -I"..\config" -I"..\kernel\knlinc"

# All of the sources participating in the build are defined here
-include sources.mk
-include mtkernel_3/lib/libtm/subdir.mk
-include mtkernel_3/lib/libtk/subdir.mk
-include mtkernel_3/kernel/usermain/subdir.mk
-include mtkernel_3/kernel/tstdlib/subdir.mk
-include mtkernel_3/kernel/tkernel/subdir.mk
-include mtkernel_3/kernel/sysinit/subdir.mk
-include mtkernel_3/kernel/inittask/subdir.mk
-include mtkernel_3/app_sample/subdir.mk

ifeq ($(TARGET), _IOTE_M367_)
include iote_m367.mk
endif
ifeq ($(TARGET), _IOTE_RX231_)
include iote_rx231.mk
endif

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

SECONDARY_SIZE += $(EXE_FILE).siz

# All EXE_FILE
all: $(EXE_FILE).elf secondary-outputs

$(EXE_FILE).elf: $(OBJS)
	@echo 'Linker: $@'
	$(GCC)  $(LFLAGS) -T $(LNKFILE) -Wl,-Map,"$(EXE_FILE).map" -o "$(EXE_FILE).elf" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' 'Ks

$(EXE_FILE).siz: $(EXE_FILE).elf
	$(SIZE) --format=berkeley "$(EXE_FILE).elf"
	@echo ' '

# Other EXE_FILEs
clean:
	-$(RM) $(OBJS)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) $(EXE_FILE).elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents
