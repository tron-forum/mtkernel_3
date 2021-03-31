################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/tx03_m367/int_m367.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/tx03_m367/ptimer_m367.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/tx03_m367/int_m367.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/tx03_m367/ptimer_m367.d 

mtkernel_3/lib/libtk/sysdepend/cpu/tx03_m367/%.o: ../lib/libtk/sysdepend/cpu/tx03_m367/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


