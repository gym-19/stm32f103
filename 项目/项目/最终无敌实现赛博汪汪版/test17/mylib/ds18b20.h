#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"

int  ds18b20_init(void);
u8 get_ds18b20_value(u8 ds18b20_data[2]);

#endif



