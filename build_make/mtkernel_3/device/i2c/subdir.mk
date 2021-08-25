################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/device/i2c/i2c.o 

C_DEPS += \
./mtkernel_3/device/i2c/i2c.d 

mtkernel_3/device/i2c/%.o: ../device/i2c/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ifeq ($(TARGET), _IOTE_M367_)
-include mtkernel_3/device/i2c/sysdepend/tx03_m367/subdir.mk
endif

ifeq ($(TARGET), _IOTE_RX231_)
-include mtkernel_3/device/i2c/sysdepend/rx231/subdir.mk
endif

ifeq ($(TARGET), _IOTE_STM32L4_)
-include mtkernel_3/device/i2c/sysdepend/stm32l4/subdir.mk
endif

#ifeq ($(TARGET), _IOTE_RZA2M_)
#-include mtkernel_3/device/i2c/sysdepend/rza2m/subdir.mk
#endif
