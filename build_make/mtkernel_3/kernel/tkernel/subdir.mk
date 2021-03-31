################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/tkernel/cpuctl.o \
./mtkernel_3/kernel/tkernel/device.o \
./mtkernel_3/kernel/tkernel/deviceio.o \
./mtkernel_3/kernel/tkernel/eventflag.o \
./mtkernel_3/kernel/tkernel/int.o \
./mtkernel_3/kernel/tkernel/klock.o \
./mtkernel_3/kernel/tkernel/mailbox.o \
./mtkernel_3/kernel/tkernel/memory.o \
./mtkernel_3/kernel/tkernel/mempfix.o \
./mtkernel_3/kernel/tkernel/mempool.o \
./mtkernel_3/kernel/tkernel/messagebuf.o \
./mtkernel_3/kernel/tkernel/misc_calls.o \
./mtkernel_3/kernel/tkernel/mutex.o \
./mtkernel_3/kernel/tkernel/objname.o \
./mtkernel_3/kernel/tkernel/power.o \
./mtkernel_3/kernel/tkernel/rendezvous.o \
./mtkernel_3/kernel/tkernel/semaphore.o \
./mtkernel_3/kernel/tkernel/task.o \
./mtkernel_3/kernel/tkernel/task_manage.o \
./mtkernel_3/kernel/tkernel/task_sync.o \
./mtkernel_3/kernel/tkernel/time_calls.o \
./mtkernel_3/kernel/tkernel/timer.o \
./mtkernel_3/kernel/tkernel/tkinit.o \
./mtkernel_3/kernel/tkernel/wait.o 

C_DEPS += \
./mtkernel_3/kernel/tkernel/cpuctl.d \
./mtkernel_3/kernel/tkernel/device.d \
./mtkernel_3/kernel/tkernel/deviceio.d \
./mtkernel_3/kernel/tkernel/eventflag.d \
./mtkernel_3/kernel/tkernel/int.d \
./mtkernel_3/kernel/tkernel/klock.d \
./mtkernel_3/kernel/tkernel/mailbox.d \
./mtkernel_3/kernel/tkernel/memory.d \
./mtkernel_3/kernel/tkernel/mempfix.d \
./mtkernel_3/kernel/tkernel/mempool.d \
./mtkernel_3/kernel/tkernel/messagebuf.d \
./mtkernel_3/kernel/tkernel/misc_calls.d \
./mtkernel_3/kernel/tkernel/mutex.d \
./mtkernel_3/kernel/tkernel/objname.d \
./mtkernel_3/kernel/tkernel/power.d \
./mtkernel_3/kernel/tkernel/rendezvous.d \
./mtkernel_3/kernel/tkernel/semaphore.d \
./mtkernel_3/kernel/tkernel/task.d \
./mtkernel_3/kernel/tkernel/task_manage.d \
./mtkernel_3/kernel/tkernel/task_sync.d \
./mtkernel_3/kernel/tkernel/time_calls.d \
./mtkernel_3/kernel/tkernel/timer.d \
./mtkernel_3/kernel/tkernel/tkinit.d \
./mtkernel_3/kernel/tkernel/wait.d 

mtkernel_3/kernel/tkernel/%.o: ../kernel/tkernel/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


