################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/usermain/usermain.o 

C_DEPS += \
./mtkernel_3/kernel/usermain/usermain.d 

mtkernel_3/kernel/usermain/%.o: ../kernel/usermain/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
