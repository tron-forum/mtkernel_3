################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/inittask/inittask.o 

C_DEPS += \
./mtkernel_3/kernel/inittask/inittask.d 

mtkernel_3/kernel/inittask/%.o: ../kernel/inittask/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


