################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/tstdlib/bitop.o \
./mtkernel_3/kernel/tstdlib/string.o 

C_DEPS += \
./mtkernel_3/kernel/tstdlib/bitop.d \
./mtkernel_3/kernel/tstdlib/string.d 


mtkernel_3/kernel/tstdlib/%.o: ../kernel/tstdlib/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
