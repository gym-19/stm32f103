#ifndef __USART_H
#define __USART_H

#include "stm32f10x_conf.h"
/*
u1_tx PA9 推挽复用
U1_RX PA10 浮空输入
*/

typedef void (*usart1_handler)(u8);

extern void usart_1_init(void);//初始化led

extern void usart_1_send_byte(u8 data);//发送一个字节的数据
extern void usart_1_send_data(u8 *data);
extern u8  usart_1_recv_byte(void);//接收一个字节的数据

extern void set_usart1_handler(usart1_handler h);

#endif




