################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysinit/sysinit.o 

C_DEPS += \
./mtkernel_3/kernel/sysinit/sysinit.d 

mtkernel_3/kernel/sysinit/%.o: ../kernel/sysinit/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


