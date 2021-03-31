################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/device/adc/sysdepend/stm32l4/adc_stm32l4.o 

C_DEPS += \
./mtkernel_3/device/adc/sysdepend/stm32l4/adc_stm32l4.d 


mtkernel_3/device/adc/sysdepend/stm32l4/%.o: ../device/adc/sysdepend/stm32l4/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


