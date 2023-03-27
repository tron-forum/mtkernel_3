################################################################################
# micro T-Kernel 3.00.06A  makefile
################################################################################

S_UPPER_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/dispatch.d \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/exc_entry.d \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/reset_hdl.d \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/int_asm.d \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/vector_tbl.d 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.d \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/exc_hdl.d \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/interrupt.d \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/reset_main.d

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/dispatch.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/exc_entry.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/reset_hdl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/int_asm.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/vector_tbl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/cpu_cntl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/exc_hdl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/interrupt.o \
./mtkernel_3/kernel/sysdepend/cpu/core/armv7a/reset_main.o

mtkernel_3/kernel/sysdepend/cpu/core/armv7a/%.o: ../kernel/sysdepend/cpu/core/armv7a/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mtkernel_3/kernel/sysdepend/cpu/core/armv7a/%.o: ../kernel/sysdepend/cpu/core/armv7a/%.S
	@echo 'Building file: $<'
	$(AS) $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


