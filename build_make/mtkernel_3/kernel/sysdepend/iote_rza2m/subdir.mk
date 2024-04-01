################################################################################
# micro T-Kernel 3.00.06A  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysdepend/iote_rza2m/cpu_clock.o \
./mtkernel_3/kernel/sysdepend/iote_rza2m/hw_setting.o \
./mtkernel_3/kernel/sysdepend/iote_rza2m/devinit.o \
./mtkernel_3/kernel/sysdepend/iote_rza2m/power_save.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/iote_rza2m/cpu_clock.d \
./mtkernel_3/kernel/sysdepend/iote_rza2m/hw_setting.d \
./mtkernel_3/kernel/sysdepend/iote_rza2m/devinit.d \
./mtkernel_3/kernel/sysdepend/iote_rza2m/power_save.d 


mtkernel_3/kernel/sysdepend/iote_rza2m/%.o: ../kernel/sysdepend/iote_rza2m/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mtkernel_3/kernel/sysdepend/iote_rza2m/%.o: ../kernel/sysdepend/iote_rza2m/%.S
	@echo 'Building file: $<'
	$(AS) $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
