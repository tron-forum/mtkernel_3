################################################################################
# Common compile rules for micro T-Kernel 3 submodules
#
# Usage: each submodule Makefile sets SRC_DIR (relative to MTKERNEL_ROOT)
#        then includes this file.
#
# Expected variables (set by top-level Makefile):
#   MTKERNEL_ROOT  — absolute path to mtkernel_3/
#   TARGET         — e.g. _IOTE_STM32L4_
#   INCPATH        — -I flags
#   GCC, AS, CFLAGS, ASFLAGS
#
# Expected variable (set by submodule Makefile before include):
#   SRC_DIR        — path relative to MTKERNEL_ROOT  (e.g. kernel/tkernel)
################################################################################

_ABS_SRC := $(MTKERNEL_ROOT)/$(SRC_DIR)
_OBJ_PFX := $(BUILD_DIR)/$(SRC_DIR)

_C_SRCS := $(wildcard $(_ABS_SRC)/*.c)
_S_SRCS := $(wildcard $(_ABS_SRC)/*.S)

_C_OBJS := $(patsubst $(_ABS_SRC)/%.c,$(_OBJ_PFX)/%.o,$(_C_SRCS))
_S_OBJS := $(patsubst $(_ABS_SRC)/%.S,$(_OBJ_PFX)/%.o,$(_S_SRCS))

OBJS += $(_C_OBJS) $(_S_OBJS)

$(_OBJ_PFX)/%.o: $(_ABS_SRC)/%.c | $(_OBJ_PFX)
	@echo 'CC  $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:.o=.d)" -MT"$@" -c -o "$@" "$<"

$(_OBJ_PFX)/%.o: $(_ABS_SRC)/%.S | $(_OBJ_PFX)
	@echo 'AS  $<'
	$(AS) $(ASFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:.o=.d)" -MT"$@" -c -o "$@" "$<"

$(_OBJ_PFX):
	mkdir -p $@
