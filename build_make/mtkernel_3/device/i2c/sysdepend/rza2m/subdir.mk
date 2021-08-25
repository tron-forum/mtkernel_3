################################################################################
# micro T-Kernel 3.00.05  makefile
################################################################################

OBJS += \
./mtkernel_3/device/i2c/sysdepend/rza2m/i2c_rza2m.o 

C_DEPS += \
./mtkernel_3/device/i2c/sysdepend/rza2m/i2c_rza2m.d 


# Each subdirectory must supply rules for building sources it contributes
mtkernel_3/device/i2c/sysdepend/rza2m/%.o: ../device/i2c/sysdepend/rza2m/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
