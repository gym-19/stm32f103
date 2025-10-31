#ifndef __KEY_MANAGER_H
#define __KEY_MANAGER_H

#include "stm32f10x.h"
#include "mkey.h"
#include "oled.h"
#include "system_manager.h"

// 菜单项定义
typedef enum {
    MENU_CALCULATOR = 0,
    MENU_STOPWATCH,
    MENU_DHT,
    // 可以在这里添加新菜单项，但要确保与system_manager.c中的menu_items顺序一致
    MENU_DS18B20,      // 新增：DS18B20温度探头菜单
		MENU_FAN_ADJUST,   // 新增：风扇调节菜单
	  MENU_CALENDAR,    //日历
    MENU_COUNT      // 这个会自动更新
} MenuItem;

uint8_t Scan_Keys(void);
void Key_Process(void);
uint8_t Scan_Independent_Keys(void);

#endif

