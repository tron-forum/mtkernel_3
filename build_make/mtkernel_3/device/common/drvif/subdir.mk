################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/device/common/drvif/msdrvif.o 

C_DEPS += \
./mtkernel_3/device/common/drvif/msdrvif.d 

mtkernel_3/device/common/drvif/%.o: ../device/common/drvif/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

