################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/tx03_m367/cpu_clock.o \
./mtkernel_3/kernel/sysdepend/cpu/tx03_m367/vector_tbl.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/tx03_m367/cpu_clock.d \
./mtkernel_3/kernel/sysdepend/cpu/tx03_m367/vector_tbl.d 

mtkernel_3/kernel/sysdepend/cpu/tx03_m367/%.o: ../kernel/sysdepend/cpu/tx03_m367/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


