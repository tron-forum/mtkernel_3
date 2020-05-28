/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	config_func.h
 *	User Configuration Definition for function
 */

#ifndef _CONFIG_FUNC_H_
#define _CONFIG_FUNC_H_

#define USE_SEMAPHORE		(1)
#define	USE_MUTEX		(1)
#define	USE_EVENTFLAG		(1)
#define	USE_MAILBOX		(1)
#define	USE_MESSAGEBUFFER	(1)
#define USE_RENDEZVOUS		(1)
#define USE_MEMORYPOOL		(1)
#define	USE_FIX_MEMORYPOOL	(1)
#define	USE_TIMEMANAGEMENT	(1)
#define	USE_CYCLICHANDLER	(1)
#define USE_ALARMHANDLER	(1)
#define USE_DEVICE		(1)
#define USE_FAST_LOCK		(1)
#define USE_MULTI_LOCK		(1)

/* Task management */
#define USE_FUNC_TK_DEL_TSK
#define USE_FUNC_TK_EXT_TSK
#define USE_FUNC_TK_EXD_TSK
#define USE_FUNC_TK_TER_TSK
#define USE_FUNC_TK_CHG_PRI
#define USE_FUNC_TK_REL_WAI
#define USE_FUNC_TK_GET_REG
#define USE_FUNC_TK_SET_REG
#define USE_FUNC_TK_GET_CPR
#define USE_FUNC_TK_SET_CPR
#define USE_FUNC_TK_REF_TSK
#define USE_FUNC_TK_SUS_TSK
#define USE_FUNC_TK_RSM_TSK
#define USE_FUNC_TK_FRSM_TSK
#define USE_FUNC_TK_SLP_TSK
#define USE_FUNC_TK_WUP_TSK
#define USE_FUNC_TK_CAN_WUP
#define USE_FUNC_TK_DLY_TSK
#define USE_FUNC_TD_LST_TSK
#define USE_FUNC_TD_REF_TSK
#define USE_FUNC_TD_INF_TSK
#define USE_FUNC_TD_GET_REG
#define USE_FUNC_TD_SET_REG

/* Semaphore management API */
#define USE_FUNC_TK_DEL_SEM
#define USE_FUNC_TK_REF_SEM
#define USE_FUNC_TD_LST_SEM
#define USE_FUNC_TD_REF_SEM
#define USE_FUNC_TD_SEM_QUE

/* Mutex management API */
#define USE_FUNC_TK_DEL_MTX
#define USE_FUNC_TK_REF_MTX
#define USE_FUNC_TD_LST_MTX
#define USE_FUNC_TD_REF_MTX
#define USE_FUNC_TD_MTX_QUE

/* Event flag management API */
#define USE_FUNC_TK_DEL_FLG
#define USE_FUNC_TK_REF_FLG
#define USE_FUNC_TD_LST_FLG
#define USE_FUNC_TD_REF_FLG
#define USE_FUNC_TD_FLG_QUE

/* Mailbox management API */
#define USE_FUNC_TK_DEL_MBX
#define USE_FUNC_TK_REF_MBX
#define USE_FUNC_TD_LST_MBX
#define USE_FUNC_TD_REF_MBX
#define USE_FUNC_TD_MBX_QUE

/* Messagebuffer management API */
#define USE_FUNC_TK_DEL_MBF
#define USE_FUNC_TK_REF_MBF
#define USE_FUNC_TD_LST_MBF
#define USE_FUNC_TD_REF_MBF
#define USE_FUNC_TD_SMBF_QUE
#define USE_FUNC_TD_RMBF_QUE

/* Rendezvous management API (Legacy API) */
#define USE_FUNC_TK_DEL_POR
#define USE_FUNC_TK_FWD_POR
#define USE_FUNC_TK_REF_POR
#define USE_FUNC_TD_LST_POR
#define USE_FUNC_TD_REF_POR
#define USE_FUNC_TD_CAL_QUE
#define USE_FUNC_TD_ACP_QUE

/* Memory pool management API */
#define USE_FUNC_TK_DEL_MPL
#define USE_FUNC_TK_REF_MPL
#define USE_FUNC_TD_LST_MPL
#define USE_FUNC_TD_REF_MPL
#define USE_FUNC_TD_MPL_QUE

/* Fix-Memory Pool management API */
#define USE_FUNC_TK_DEL_MPF
#define USE_FUNC_TK_REF_MPF
#define USE_FUNC_TD_LST_MPF
#define USE_FUNC_TD_REF_MPF
#define USE_FUNC_TD_MPF_QUE

/* Time management API */
#define USE_FUNC_TK_SET_UTC
#define USE_FUNC_TK_GET_UTC
#define USE_FUNC_TK_SET_TIM
#define USE_FUNC_TK_GET_TIM
#define USE_FUNC_TK_GET_OTM
#define USE_FUNC_TD_GET_TIM
#define USE_FUNC_TD_GET_OTM

/* Cyclic handler management API */
#define USE_FUNC_TK_DEL_CYC
#define USE_FUNC_TK_STA_CYC
#define USE_FUNC_TK_STP_CYC
#define USE_FUNC_TK_REF_CYC
#define USE_FUNC_TD_LST_CYC
#define USE_FUNC_TD_REF_CYC

/* Alarm handler management API */
#define USE_FUNC_TK_DEL_ALM
#define USE_FUNC_TK_STP_ALM
#define USE_FUNC_TK_REF_ALM
#define USE_FUNC_TD_LST_ALM
#define USE_FUNC_TD_REF_ALM

/* System status management API */
#define USE_FUNC_TK_ROT_RDQ
#define USE_FUNC_TK_GET_TID
#define USE_FUNC_TK_DIS_DSP
#define USE_FUNC_TK_ENA_DSP
#define USE_FUNC_TK_REF_SYS
#define USE_FUNC_TK_REF_VER
#define USE_FUNC_TD_REF_SYS
#define USE_FUNC_TD_RDY_QUE

#endif /* _CONFIG_FUNC_H_ */
