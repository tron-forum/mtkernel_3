################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

S_UPPER_SRCS += \
../kernel/sysdepend/cpu/rx231/hllint_ent.S 

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/rx231/hllint_ent.o \
./mtkernel_3/kernel/sysdepend/cpu/rx231/hllint_tbl.o \
./mtkernel_3/kernel/sysdepend/cpu/rx231/intvect_tbl.o 

S_UPPER_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/rx231/hllint_ent.d 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/rx231/hllint_tbl.d \
./mtkernel_3/kernel/sysdepend/cpu/rx231/intvect_tbl.d 

mtkernel_3/kernel/sysdepend/cpu/rx231/%.o: ../kernel/sysdepend/cpu/rx231/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mtkernel_3/kernel/sysdepend/cpu/rx231/%.o: ../kernel/sysdepend/cpu/rx231/%.S
	@echo 'Building file: $<'
	$(AS) $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
