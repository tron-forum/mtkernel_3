################################################################################
# micro T-Kernel 3.00.06A  makefile
################################################################################

S_UPPER_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/rza2m/sf_boot.d \

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/rza2m/ttb_ini.d

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/rza2m/sf_boot.o \
./mtkernel_3/kernel/sysdepend/cpu/rza2m/ttb_ini.o

mtkernel_3/kernel/sysdepend/cpu/rza2m/%.o: ../kernel/sysdepend/cpu/rza2m/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mtkernel_3/kernel/sysdepend/cpu/rza2m/%.o: ../kernel/sysdepend/cpu/rza2m/%.S
	@echo 'Building file: $<'
	$(AS) $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
