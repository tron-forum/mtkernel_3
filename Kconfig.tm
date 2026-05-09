# Kconfig.tm — µT-Kernel T-Monitor compatible API
#
# Corresponds to: mtkernel_3/config/config_tm.h

config USE_TMONITOR
	bool "Use T-Monitor compatible API"
	default y

config USE_SYSTEM_MESSAGE
	bool "Output system messages via T-Monitor"
	default y

config USE_EXCEPTION_DBG_MSG
	bool "Output exception debug messages"
	default y

config USE_TASK_DBG_MSG
	bool "Output task debug messages"
	default n

choice
	prompt "T-Monitor communication port"
	default TM_COM_SERIAL_DEV

config TM_COM_SERIAL_DEV
	bool "Serial communication device"

config TM_COM_NO_DEV
	bool "No communication port (silent)"

endchoice

config USE_TM_PRINTF
	bool "Enable tm_printf() and tm_sprintf()"
	default y

config TM_OUTBUF_SZ
	int "T-Monitor output buffer size in stack (0 = no buffering)"
	default 0
