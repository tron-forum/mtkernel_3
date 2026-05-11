################################################################################
# lib/libtm — common + sysdepend
################################################################################

# Common sources
SRC_DIR := lib/libtm
include $(MTKERNEL_ROOT)/make/rules.mk

# Board layer
SRC_DIR := lib/libtm/sysdepend/$(BOARD_DIR)
include $(MTKERNEL_ROOT)/make/rules.mk

# no_device fallback
SRC_DIR := lib/libtm/sysdepend/no_device
include $(MTKERNEL_ROOT)/make/rules.mk
