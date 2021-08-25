################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/device/adc/adc.o 

C_DEPS += \
./mtkernel_3/device/adc/adc.d 

mtkernel_3/device/adc/%.o: ../device/adc/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ifeq ($(TARGET), _IOTE_M367_)
-include mtkernel_3/device/adc/sysdepend/tx03_m367/subdir.mk
endif

ifeq ($(TARGET), _IOTE_RX231_)
-include mtkernel_3/device/adc/sysdepend/rx231/subdir.mk
endif

ifeq ($(TARGET), _IOTE_STM32L4_)
-include mtkernel_3/device/adc/sysdepend/stm32l4/subdir.mk
endif

ifeq ($(TARGET), _IOTE_RZA2M_)
-include mtkernel_3/device/adc/sysdepend/rza2m/subdir.mk
endif
