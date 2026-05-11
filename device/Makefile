################################################################################
# device — aggregates all device drivers
################################################################################

# Common driver interface
SRC_DIR := device/common/drvif
include $(MTKERNEL_ROOT)/make/rules.mk

# Individual device drivers — each has common + sysdepend/$(CPU_DIR)
DEVICE_DRIVERS := ser i2c adc

define include_device_driver
SRC_DIR := device/$(1)
include $(MTKERNEL_ROOT)/make/rules.mk

# sysdepend is optional (not all drivers exist for all CPUs)
ifneq ($$(wildcard $(MTKERNEL_ROOT)/device/$(1)/sysdepend/$(CPU_DIR)),)
SRC_DIR := device/$(1)/sysdepend/$(CPU_DIR)
include $(MTKERNEL_ROOT)/make/rules.mk
endif
endef

$(foreach drv,$(DEVICE_DRIVERS),$(eval $(call include_device_driver,$(drv))))
