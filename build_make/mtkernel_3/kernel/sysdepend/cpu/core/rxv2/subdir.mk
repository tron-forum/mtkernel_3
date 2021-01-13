################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

S_UPPER_SRCS += \
../kernel/sysdepend/cpu/core/rxv2/dispatch.S \
../kernel/sysdepend/cpu/core/rxv2/int_asm.S \
../kernel/sysdepend/cpu/core/rxv2/reset_hdl.S 

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/cpu_cntl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/dispatch.o \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/exc_hdr.o \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/int_asm.o \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/interrupt.o \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/reset_hdl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/reset_main.o \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/vector_tbl.o 

S_UPPER_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/dispatch.d \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/int_asm.d \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/reset_hdl.d 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/cpu_cntl.d \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/exc_hdr.d \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/interrupt.d \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/reset_main.d \
./mtkernel_3/kernel/sysdepend/cpu/core/rxv2/vector_tbl.d 

mtkernel_3/kernel/sysdepend/cpu/core/rxv2/%.o: ../kernel/sysdepend/cpu/core/rxv2/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mtkernel_3/kernel/sysdepend/cpu/core/rxv2/%.o: ../kernel/sysdepend/cpu/core/rxv2/%.S
	@echo 'Building file: $<'
	$(AS) $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
