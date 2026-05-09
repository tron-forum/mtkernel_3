################################################################################
# kernel/sysdepend — collects board, cpu, and core sources via BOARD_DIR etc.
################################################################################

# Board layer
SRC_DIR := kernel/sysdepend/$(BOARD_DIR)
include $(MTKERNEL_ROOT)/config/rules.mk

# CPU layer
SRC_DIR := kernel/sysdepend/cpu/$(CPU_DIR)
include $(MTKERNEL_ROOT)/config/rules.mk

# Core layer
SRC_DIR := kernel/sysdepend/cpu/core/$(CORE_DIR)
include $(MTKERNEL_ROOT)/config/rules.mk
