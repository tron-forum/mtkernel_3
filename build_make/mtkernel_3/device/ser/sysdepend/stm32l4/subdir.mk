################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/device/ser/sysdepend/stm32l4/ser_stm32l4.o 

C_DEPS += \
./mtkernel_3/device/ser/sysdepend/stm32l4/ser_stm32l4.d 


# Each subdirectory must supply rules for building sources it contributes
mtkernel_3/device/ser/sysdepend/stm32l4/%.o: ../mtkernel_3/device/ser/sysdepend/stm32l4/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


