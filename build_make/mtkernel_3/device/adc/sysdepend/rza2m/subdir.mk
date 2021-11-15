################################################################################
# micro T-Kernel 3.00.05  makefile
################################################################################

C_DEPS += \
./mtkernel_3/device/adc/sysdepend/rza2m/adc_rza2m.d 

OBJS += \
./mtkernel_3/device/adc/sysdepend/rza2m/adc_rza2m.o 


mtkernel_3/device/adc/sysdepend/rza2m/%.o: ../device/adc/sysdepend/rza2m/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
