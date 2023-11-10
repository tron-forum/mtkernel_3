/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *	Machine type definition 
 */

#ifndef __SYS_MACHINE_H__
#define __SYS_MACHINE_H__

/* ===== System dependencies definitions ================================ */

#ifdef _IOTE_M367_
#include "sysdepend/iote_m367/machine.h"

#define Csym(sym) sym

#endif

#ifdef _IOTE_STM32L4_
#include "sysdepend/iote_stm32l4/machine.h"

#define Csym(sym) sym

#endif

#ifdef _IOTE_RX231_
#include "sysdepend/iote_rx231/machine.h"

#define Csym(sym) _##sym

#endif

#ifdef _IOTE_RZA2M_
#include "sysdepend/iote_rza2m/machine.h"

#define Csym(sym) sym

#endif

/* ===== C compiler dependencies definitions ============================= */

#ifdef __GNUC__

#define Inline static __inline__
#define Asm __asm__ volatile
#define Noinit(decl) decl __attribute__((section(".noinit")))
#define	Section(decl,name) decl __attribute__((section(#name)))
#define WEAK_FUNC __attribute__((weak))

#define _VECTOR_ENTRY(name) .word name
#define _WEAK_ENTRY(name) .weak name

#endif /* __GNUC__ */

#endif /* __SYS_MACHINE_H__ */
