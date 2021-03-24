################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

C_DEPS += \
./mtkernel_3/device/ser/sysdepend/rx231/ser_rx231.d 

OBJS += \
./mtkernel_3/device/ser/sysdepend/rx231/ser_rx231.o 


# Each subdirectory must supply rules for building sources it contributes
mtkernel_3/device/ser/sysdepend/rx231/%.o: ../mtkernel_3/device/ser/sysdepend/rx231/%.c
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(file > $@.in,-O0 -ffunction-sections -fdata-sections -Wall -g2 -mcpu=rx230 -mlittle-endian-data -I"D:\\tkernel_dev\\release_30003_rx\\mtkernel_3\\include" -I"D:\\tkernel_dev\\release_30003_rx\\mtkernel_3\\config" -I"D:\\tkernel_dev\\release_30003_rx\\mtkernel_3\\kernel\\knlinc" -D_IOTE_RX231_ -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@")
	@echo 'rx-elf-gcc -O0 -ffunction-sections -fdata-sections -Wall -g2 -mcpu=rx230 -mlittle-endian-data -I"D:\\tkernel_dev\\release_30003_rx\\mtkernel_3\\include" -I"D:\\tkernel_dev\\release_30003_rx\\mtkernel_3\\config" -I"D:\\tkernel_dev\\release_30003_rx\\mtkernel_3\\kernel\\knlinc" -D_IOTE_RX231_ -Wa,-adlnh="$(basename $(notdir $<)).lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" "$<" -c -o "$@"'

	rx-elf-gcc @"$@.in"
	@echo 'Finished building: $<'
	@echo ' '


