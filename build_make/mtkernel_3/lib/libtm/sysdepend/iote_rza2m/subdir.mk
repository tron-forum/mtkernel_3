################################################################################
# micro T-Kernel 3.00.05  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtm/sysdepend/iote_rza2m/tm_com.o 

C_DEPS += \
./mtkernel_3/lib/libtm/sysdepend/iote_rza2m/tm_com.d 

mtkernel_3/lib/libtm/sysdepend/iote_rza2m/%.o: ../lib/libtm/sysdepend/iote_rza2m/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


