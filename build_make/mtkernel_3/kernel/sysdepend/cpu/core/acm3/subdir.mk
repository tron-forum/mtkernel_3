################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

S_UPPER_SRCS += \
../kernel/sysdepend/cpu/core/acm3/dispatch.S 

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/cpu_cntl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/dispatch.o \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/exc_hdr.o \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/interrupt.o \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/reset_hdl.o \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/vector_tbl.o 

S_UPPER_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/dispatch.d 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/cpu_cntl.d \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/exc_hdr.d \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/interrupt.d \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/reset_hdl.d \
./mtkernel_3/kernel/sysdepend/cpu/core/acm3/vector_tbl.d 


mtkernel_3/kernel/sysdepend/cpu/core/acm3/%.o: ../kernel/sysdepend/cpu/core/acm3/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mtkernel_3/kernel/sysdepend/cpu/core/acm3/%.o: ../kernel/sysdepend/cpu/core/acm3/%.S
	@echo 'Building file: $<'
	$(AS)  $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


