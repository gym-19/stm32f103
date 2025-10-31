#ifndef __LDT_H
#define __LDT_H

#include "stm32f10x_conf.h"
/*

*/



extern void ldt_init(void);//≥ı ºªØled

extern void hc138_out_data(u8 data);

extern void hc595_send_data(u8 data);

extern void digit_show_data(int data);
#endif




