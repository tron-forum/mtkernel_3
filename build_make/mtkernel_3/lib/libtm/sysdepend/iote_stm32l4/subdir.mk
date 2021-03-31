################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtm/sysdepend/iote_stm32l4/tm_com.o 

C_DEPS += \
./mtkernel_3/lib/libtm/sysdepend/iote_stm32l4/tm_com.d 

mtkernel_3/lib/libtm/sysdepend/iote_stm32l4/%.o: ../lib/libtm/sysdepend/iote_stm32l4/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


