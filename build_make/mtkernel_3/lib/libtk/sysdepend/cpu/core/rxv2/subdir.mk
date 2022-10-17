################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/rxv2/int_rxv2.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/rxv2/wusec_rvx2.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/rxv2/int_rxv2.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/core/rxv2/wusec_rvx2.d 

mtkernel_3/lib/libtk/sysdepend/cpu/core/rxv2/%.o: ../lib/libtk/sysdepend/cpu/core/rxv2/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


