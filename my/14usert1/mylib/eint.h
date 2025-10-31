#ifndef __EINT_H
#define __EINT_H

#include "stm32f10x_conf.h" 
/*
KEY0 PC9 EXTI9 EXTI9_5
KEY1 PC8 EXTI8 EXTI9_5
KEY2 PA0 EXTI0 EXTI0
*/
typedef void (*eint_handler)(void);
	
extern void eint_init(void);	//≥ı ºªØ

extern void set_eint_handler(eint_handler h0, eint_handler h1, eint_handler h2);
#endif

