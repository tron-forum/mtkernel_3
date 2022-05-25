################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/stm32l4/vector_tbl.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/stm32l4/vector_tbl.d 

mtkernel_3/kernel/sysdepend/cpu/stm32l4/%.o: ../kernel/sysdepend/cpu/stm32l4/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


