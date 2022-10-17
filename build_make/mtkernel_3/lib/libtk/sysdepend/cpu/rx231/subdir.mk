################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/rx231/int_rx231.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/rx231/ipr_rx231.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/rx231/ptimer_rx231.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/rx231/int_rx231.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/rx231/ipr_rx231.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/rx231/ptimer_rx231.d 

mtkernel_3/lib/libtk/sysdepend/cpu/rx231/%.o: ../lib/libtk/sysdepend/cpu/rx231/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


