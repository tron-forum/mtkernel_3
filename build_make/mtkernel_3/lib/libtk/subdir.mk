################################################################################
# micro T-Kernel 3.00.01  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/fastlock.o \
./mtkernel_3/lib/libtk/fastmlock.o 

C_DEPS += \
./mtkernel_3/lib/libtk/fastlock.d \
./mtkernel_3/lib/libtk/fastmlock.d 

mtkernel_3/lib/libtk/%.o: ../lib/libtk/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


