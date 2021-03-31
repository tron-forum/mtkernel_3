################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtm/libtm.o \
./mtkernel_3/lib/libtm/libtm_printf.o 

C_DEPS += \
./mtkernel_3/lib/libtm/libtm.d \
./mtkernel_3/lib/libtm/libtm_printf.d 

mtkernel_3/lib/libtm/%.o: ../lib/libtm/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


