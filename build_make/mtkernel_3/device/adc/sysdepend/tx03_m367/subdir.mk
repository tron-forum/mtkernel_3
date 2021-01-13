################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/device/adc/sysdepend/tx03_m367/adc_m367.o 

C_DEPS += \
./mtkernel_3/device/adc/sysdepend/tx03_m367/adc_m367.d 


mtkernel_3/device/adc/sysdepend/tx03_m367/%.o: ../device/adc/sysdepend/tx03_m367/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


