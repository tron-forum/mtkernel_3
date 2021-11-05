################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/rza2m/ptimer_rza2m.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/rza2m/ptimer_rza2m.d 

mtkernel_3/lib/libtk/sysdepend/cpu/rza2m/%.o: ../lib/libtk/sysdepend/cpu/rza2m/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
