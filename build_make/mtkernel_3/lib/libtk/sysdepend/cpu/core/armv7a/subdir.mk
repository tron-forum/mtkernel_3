################################################################################
# micro T-Kernel 3.00.05  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/int_armv7a.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/wusec_armv7a.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/int_armv7a.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/wusec_armv7a.d 


mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/%.o: ../lib/libtk/sysdepend/cpu/core/armv7m/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
