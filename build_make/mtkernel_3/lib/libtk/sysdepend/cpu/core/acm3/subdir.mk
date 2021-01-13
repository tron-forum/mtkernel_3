################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/acm3/int_acm3.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/acm3/wusec_acm3.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/acm3/int_acm3.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/acm3/wusec_acm3.d 

mtkernel_3/lib/libtk/sysdepend/cpu/core/acm3/%.o: ../lib/libtk/sysdepend/cpu/core/acm3/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


